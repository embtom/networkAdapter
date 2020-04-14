/*
 * This file is part of the EMBTOM project
 * Copyright (c) 2018-2019 Thomas Willetal
 * (https://github.com/embtom)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//******************************************************************************
// Header

#include <iostream>
#include <stdexcept>

#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <fdSet.h>
#include <error_msg.hpp>
#include <BaseSocket.hpp>
#include <Tcp/TcpDataLink.hpp>

namespace EtNet
{
    class CTcpDataLinkPrivate : public std::enable_shared_from_this<CTcpDataLinkPrivate>
    {
    public:
        CTcpDataLinkPrivate() = default;
        CTcpDataLinkPrivate(int socketFd) noexcept;
        CTcpDataLinkPrivate(CBaseSocket&& rBaseSocket) noexcept;
        ~CTcpDataLinkPrivate() noexcept;
        void send(const utils::span<uint8_t>& rTxSpan) const;

        bool unblockRecive() noexcept;
        CTcpDataLink::ERet recive(utils::span<uint8_t>& rRxSpan, CTcpDataLink::CallbackReceive scanForEnd);

    private:
        void reciveImpl(utils::span<uint8_t>& rRxSpan, CTcpDataLink::CallbackReceive scanForEnd);

        utils::CFdSet   m_FdSet;
        CBaseSocket     m_baseSocket;
    };
}

using namespace EtNet;

//*****************************************************************************
// Method definitions "CTcpDataLinkPrivate"

CTcpDataLinkPrivate::CTcpDataLinkPrivate(int socketFd) noexcept :
    m_baseSocket(socketFd)
{
    m_FdSet.AddFd(socketFd);
}

CTcpDataLinkPrivate::CTcpDataLinkPrivate(CBaseSocket&& rBaseSocket) noexcept :
    m_baseSocket(std::move(rBaseSocket))
{
    m_FdSet.AddFd(m_baseSocket.getFd());
}


CTcpDataLinkPrivate::~CTcpDataLinkPrivate() noexcept
{
    if (!m_baseSocket.isValid()) {
        return;
    }

    unblockRecive();
    try {
        m_FdSet.RemoveFd(m_baseSocket.getFd());
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }
}

void CTcpDataLinkPrivate::send(const utils::span<uint8_t>& rTxSpan) const
{
    std::size_t dataWritten = 0;

    while(dataWritten < rTxSpan.size_bytes())
    {
        std::size_t put = ::send(m_baseSocket.getFd(), rTxSpan.data() + dataWritten, rTxSpan.size_bytes() - dataWritten, 0);
        if (put == static_cast<std::size_t>(-1))
        {
            switch(errno)
            {
                case EINVAL:     [[fallthrough]];
                case EBADF:      [[fallthrough]];
                case ECONNRESET: [[fallthrough]];
                case ENXIO:      [[fallthrough]];
                case EPIPE:
                {
                    // Fatal error
                    throw std::domain_error(utils::buildErrorMessage("DataSocket::", __func__, ": write: critical error: ", strerror(errno)));
                }
                case EDQUOT:     [[fallthrough]];
                case EFBIG:      [[fallthrough]];
                case EIO:        [[fallthrough]];
                case ENETDOWN:   [[fallthrough]];
                case ENETUNREACH:[[fallthrough]];
                case ENOSPC:
                {
                    // Resource acquisition failure or device error
                    throw std::runtime_error(utils::buildErrorMessage("DataSocket::", __func__, ": write: resource failure: ", strerror(errno)));
                }
                case EINTR:      [[fallthrough]];
                        // TODO: Check for user interrupt flags.
                        //       Beyond the scope of this project
                        //       so continue normal operations.
                case EAGAIN:
                {
                    // Temporary Error, retry the read.
                    continue;
                }
                default:
                {
                    throw std::runtime_error(utils::buildErrorMessage("DataSocket::", __func__, ": write: returned -1: ", strerror(errno)));
                }
            }
        }
        dataWritten += put;
    }
    return;
}

bool CTcpDataLinkPrivate::unblockRecive() noexcept
{
    try {
        m_FdSet.UnBlock();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

CTcpDataLink::ERet CTcpDataLinkPrivate::recive(utils::span<uint8_t>& rSpanRx, CTcpDataLink::CallbackReceive scanForEnd)
{
    utils::CFdSetRetval ret = m_FdSet.Select([this, &rSpanRx, &scanForEnd](int fd) {
        reciveImpl(rSpanRx, scanForEnd);
    });

    switch(ret)
    {
        case utils::CFdSetRetval::UNBLOCK:  return CTcpDataLink::ERet::UNBLOCK;
        case utils::CFdSetRetval::OK :      return CTcpDataLink::ERet::OK;
    }
}

void CTcpDataLinkPrivate::reciveImpl(utils::span<uint8_t>& rRxSpan, CTcpDataLink::CallbackReceive scanForEnd)
{
    if (m_baseSocket.getFd() == 0)
    {
        throw std::logic_error(utils::buildErrorMessage("DataSocket::", __func__, ": accept called on a bad socket object (this object was moved)"));
    }

    std::size_t dataRead  = 0;
    uint8_t* readBuffer = rRxSpan.data();

    while(dataRead < rRxSpan.size_bytes())
    {
        // The inner loop handles interactions with the socket.
        std::size_t get = recv(m_baseSocket.getFd(), readBuffer + dataRead, rRxSpan.size_bytes() - dataRead, 0);
        if (get == static_cast<std::size_t>(-1))
        {
            switch(errno)
            {
                case EBADF:     [[fallthrough]];
                case EFAULT:    [[fallthrough]];
                case EINVAL:    [[fallthrough]];
                case ENXIO:
                {
                    // Fatal error. Programming bug
                    throw std::domain_error(utils::buildErrorMessage("DataSocket::", __func__, ": read: critical error: ", strerror(errno)));
                }

                case EIO:       [[fallthrough]];
                case ENOBUFS:   [[fallthrough]];
                case ENOMEM:
                {
                   // Resource acquisition failure or device error
                    throw std::runtime_error(utils::buildErrorMessage("DataSocket::", __func__, ": read: resource failure: ", strerror(errno)));
                }
                case EINTR:     [[fallthrough]];
                    // TODO: Check for user interrupt flags.
                    //       Beyond the scope of this project
                    //       so continue normal operations.
                case ETIMEDOUT: [[fallthrough]];
                case EAGAIN:
                {
                    // Temporary error, retry
                    continue;
                }
                case ECONNRESET:[[fallthrough]];
                case ENOTCONN:
                {
                    std::cout << "ENOTCONN"<< std::endl;    

                    // Connection broken.
                    // Return the data we have available and exit
                    // as if the connection was closed correctly.
                    get = 0;
                    break;
                }
                default:
                {
                    throw std::runtime_error(utils::buildErrorMessage("DataSocket::", __func__, ": read: returned -1: ", strerror(errno)));
                }
            }
        }
        if (get == 0)
        {
            break;
        }
        dataRead += get;
        if (scanForEnd(utils::span<uint8_t>(readBuffer, dataRead)))
        {
            break;
        }
    }
    rRxSpan = utils::span<uint8_t>(readBuffer, dataRead);
}

//*****************************************************************************
// Method definitions "CTcpDataLink"

CTcpDataLink::CTcpDataLink(int socketFd) noexcept :
    m_pPrivate(std::make_shared<CTcpDataLinkPrivate>(socketFd))
{ }

CTcpDataLink::CTcpDataLink(CBaseSocket&& rBaseSocket) noexcept :
    m_pPrivate(std::make_shared<CTcpDataLinkPrivate>(std::move(rBaseSocket)))
{ }

CTcpDataLink::~CTcpDataLink() noexcept = default;

CTcpDataLink::CTcpDataLink(CTcpDataLink &&rhs) noexcept :
    m_pPrivate(std::move(rhs.m_pPrivate))
{ }

CTcpDataLink& CTcpDataLink::operator=(CTcpDataLink&& rhs) noexcept
{
    m_pPrivate = std::move(rhs.m_pPrivate);
    return *this;
}

void CTcpDataLink::send(const utils::span<uint8_t>& rTxSpan) const
{
    m_pPrivate->send(rTxSpan);
}

bool CTcpDataLink::unblockRecive() noexcept
{
    return m_pPrivate->unblockRecive();
}

CTcpDataLink::ERet CTcpDataLink::recive(utils::span<uint8_t>& rRxSpan, CallbackReceive scanForEnd)
{
    return m_pPrivate->recive(rRxSpan, scanForEnd);
}

CTcpDataLink::ERet CTcpDataLink::recive(utils::span<uint8_t>&& rRxSpan, CallbackReceive scanForEnd)
{
    return m_pPrivate->recive(rRxSpan, scanForEnd);
}



