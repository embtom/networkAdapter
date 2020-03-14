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

#include <fdSet.h>
#include <error_msg.hpp>
#include <Stream/StreamDataLink.hpp>

namespace EtNet
{
    class CStreamDataLinkPrivate : public std::enable_shared_from_this<CStreamDataLinkPrivate>
    {
    public:
        CStreamDataLinkPrivate() noexcept = default;
        CStreamDataLinkPrivate(int socketFd) noexcept;
        ~CStreamDataLinkPrivate() noexcept;
        void send(const utils::span<char>& rTxSpan);

        bool unblockRecive() noexcept;
        CStreamDataLink::ERet recive(utils::span<char>& rRxSpan, CStreamDataLink::CallbackReceive scanForEnd);

    private:
        void reciveImpl(utils::span<char>& rRxSpan, CStreamDataLink::CallbackReceive scanForEnd);

        utils::CFdSet   m_FdSet;
        int             m_socketFd {-1};
    };
}

using namespace EtNet;

//*****************************************************************************
// Method definitions "CStreamDataLinkPrivate"

CStreamDataLinkPrivate::CStreamDataLinkPrivate(int socketFd) noexcept :
    m_socketFd(socketFd)
{
    m_FdSet.AddFd(socketFd);
}

CStreamDataLinkPrivate::~CStreamDataLinkPrivate() noexcept
{
    if (m_socketFd <= 0) {
        return;
    }
    unblockRecive();
    try {
        m_FdSet.RemoveFd(m_socketFd);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }
    close (m_socketFd);
}

void CStreamDataLinkPrivate::send(const utils::span<char>& rTxSpan)
{
    std::size_t dataWritten = 0;

    while(dataWritten < rTxSpan.size_bytes())
    {
        std::size_t put = write(m_socketFd, rTxSpan.data() + dataWritten, rTxSpan.size_bytes() - dataWritten);
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

bool CStreamDataLinkPrivate::unblockRecive() noexcept
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

CStreamDataLink::ERet CStreamDataLinkPrivate::recive(utils::span<char>& rSpanRx, CStreamDataLink::CallbackReceive scanForEnd)
{
    utils::CFdSetRetval ret = m_FdSet.Select([this, &rSpanRx, &scanForEnd](int fd) {
        reciveImpl(rSpanRx, scanForEnd);
    });

    switch(ret)
    {
        case utils::CFdSetRetval::UNBLOCK:  return CStreamDataLink::ERet::UNBLOCK;
        case utils::CFdSetRetval::OK :      return CStreamDataLink::ERet::OK;
    }
}

void CStreamDataLinkPrivate::reciveImpl(utils::span<char>& rRxSpan, CStreamDataLink::CallbackReceive scanForEnd)
{
    if (m_socketFd == 0)
    {
        throw std::logic_error(utils::buildErrorMessage("DataSocket::", __func__, ": accept called on a bad socket object (this object was moved)"));
    }

    std::size_t dataRead  = 0;
    char* readBuffer = rRxSpan.data();

    while(dataRead < rRxSpan.size_bytes())
    {
        // The inner loop handles interactions with the socket.
        std::size_t get = read(m_socketFd, readBuffer + dataRead, rRxSpan.size_bytes() - dataRead);
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
        if (scanForEnd(utils::span<char>(readBuffer, dataRead)))
        {
            break;
        }
    }
    rRxSpan = utils::span<char>(readBuffer, dataRead);
}

//*****************************************************************************
// Method definitions "CStreamDataLink"

CStreamDataLink::CStreamDataLink(int socketFd) noexcept :
    m_pPrivate(std::make_shared<CStreamDataLinkPrivate>(socketFd))
{ }

CStreamDataLink::~CStreamDataLink() noexcept = default;

CStreamDataLink::CStreamDataLink(CStreamDataLink &&rhs) noexcept :
    m_pPrivate(std::move(rhs.m_pPrivate))
{ }

CStreamDataLink& CStreamDataLink::operator=(CStreamDataLink&& rhs) noexcept
{
    m_pPrivate = std::move(rhs.m_pPrivate);
    return *this;
}

void CStreamDataLink::send(const utils::span<char>& rTxSpan)
{
    m_pPrivate->send(rTxSpan);
}

bool CStreamDataLink::unblockRecive() noexcept
{
    return m_pPrivate->unblockRecive();
}

CStreamDataLink::ERet CStreamDataLink::recive(utils::span<char>& rRxSpan, CallbackReceive scanForEnd)
{
    return m_pPrivate->recive(rRxSpan, scanForEnd);
}


