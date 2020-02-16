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
#include <Stream/StreamServer.hpp>
#include <BaseSocket.hpp>
#include <stdexcept>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#include <error_msg.hpp>
#include <make_unordered_map.h>
#include <BaseSocket.hpp>

namespace EtNet
{
//*****************************************************************************
//! \brief CStreamServerPrivate
//!
class CStreamServerPrivate
{
public:
    CStreamServerPrivate(CBaseSocket&& rBaseSocket, unsigned int port);
    std::tuple<CStreamDataLink, CIpAddress> waitForConnection();
private:
    CBaseSocket m_baseSocket;

};


}

using namespace EtNet;

//*****************************************************************************
// Method definitions "CStreamServerPrivate"

CStreamServerPrivate::CStreamServerPrivate(CBaseSocket&& rBaseSocket, unsigned int port) :
    m_baseSocket(std::move(rBaseSocket))
{
    int fd = m_baseSocket.getFd();
    auto setupServer = [fd](const sockaddr* addr, socklen_t len)
    {
        if (::bind(fd, addr, len) != 0)
        {
            throw std::runtime_error(utils::buildErrorMessage("ServerSocket::", __func__, ": bind: ", strerror(errno)));
        }

        if (::listen(fd, maxConnectionBacklog) != 0)
        {
            throw std::runtime_error(utils::buildErrorMessage("ServerSocket::", __func__, ": listen: ", strerror(errno)));
        }
    };

    int domain;
    int length = sizeof(int);
    getsockopt(fd, SOL_SOCKET, SO_DOMAIN, &domain, (socklen_t*)&length);

    switch (domain)
    {
        case AF_INET:
        {
            sockaddr_in serverAddr {0};
            serverAddr.sin_family       = AF_INET;
            serverAddr.sin_port         = htons(port);
            serverAddr.sin_addr.s_addr  = INADDR_ANY;
            setupServer(reinterpret_cast<sockaddr*>(&serverAddr), sizeof(sockaddr_in));
            break;
        }
        case AF_INET6:
        {
            sockaddr_in6 serverAddr {0};
            serverAddr.sin6_family       = AF_INET6;
            serverAddr.sin6_port         = htons(port);
            serverAddr.sin6_addr         = in6addr_any;
            setupServer(reinterpret_cast<sockaddr*>(&serverAddr), sizeof(sockaddr_in6));
            break;
        }
        default:
        {
            throw std::logic_error(utils::buildErrorMessage("ServerSocket::", __func__));
            break;
        }

    }
}

std::tuple<CStreamDataLink, CIpAddress> CStreamServerPrivate::waitForConnection()
{
    union
    {
        sockaddr_storage  common;
        sockaddr_in       sin;
        sockaddr_in6      sin6;
    } peerAdr;

    socklen_t addr_size   = sizeof(peerAdr);
    int newSocket = ::accept(m_baseSocket.getFd(), (struct sockaddr*)&peerAdr, &addr_size);

    CIpAddress peerAddress;
    switch (peerAdr.common.ss_family)
    {
        case AF_INET:
        {
            peerAddress = std::move(CIpAddress(peerAdr.sin.sin_addr));
            break;
        }
        case AF_INET6:
        {
            if (IN6_IS_ADDR_V4MAPPED(&peerAdr.sin6.sin6_addr)) {
                in_addr ip4;
                std::memcpy(&ip4, &peerAdr.sin6.sin6_addr.__in6_u.__u6_addr8[12], sizeof(in_addr));
                peerAddress = std::move(CIpAddress(ip4));
            }
            else {
                peerAddress = std::move(CIpAddress(peerAdr.sin6.sin6_addr));
            }
        }
        break;
    }

    if (newSocket == -1)
    {
        throw std::runtime_error(utils::buildErrorMessage("ServerSocket:", __func__, ": accept: ", strerror(errno)));
    }
    return std::tuple(CStreamDataLink(newSocket),peerAddress);
}

//*****************************************************************************
// Method definitions "CStreamServer"

void CStreamServer::privateDeleterHook(CStreamServerPrivate *it)
{
    delete it;
}

CStreamServer::CStreamServer(CBaseSocket&& rBaseSocket, unsigned int port) :
    m_pPrivate(new CStreamServerPrivate(std::move(rBaseSocket), port))
{ }

std::tuple<CStreamDataLink, CIpAddress> CStreamServer::waitForConnection()
{
    return m_pPrivate->waitForConnection();
}
