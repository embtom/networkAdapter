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

#include <Udp/UdpServer.hpp>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include <stdexcept>
#include <future>

#include <error_msg.hpp>
#include <fdSet.h>
#include <BaseSocket.hpp>

namespace EtNet
{

//*****************************************************************************
//! \brief CUdpClientPrivate
//!
class CUdpServerPrivate
{
public:
    CUdpServerPrivate(CBaseSocket&& rBaseSocket, unsigned int port);
    ~CUdpServerPrivate();
    CUdpDataLink waitForConnection();

private:
    utils::CFdSet       m_FdSet;
    CBaseSocket         m_baseSocket;
};

}

using namespace EtNet;

//*****************************************************************************
// Method definitions "CUdpServerPrivate"

CUdpServerPrivate::CUdpServerPrivate(CBaseSocket&& rBaseSocket, unsigned int port) :
    m_baseSocket(std::move(rBaseSocket))
{
    int fd = m_baseSocket.getFd();
    auto setupServer = [fd](const sockaddr* addr, socklen_t len)
    {
        if (::bind(fd, addr, len) != 0)
        {
            throw std::runtime_error(utils::buildErrorMessage("ServerSocket::", __func__, ": bind: ", strerror(errno)));
        }
    };

    switch (m_baseSocket.getDomain())
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

CUdpServerPrivate::~CUdpServerPrivate()
{
    m_FdSet.UnBlock();
}

CUdpDataLink CUdpServerPrivate::waitForConnection()
{
    m_FdSet.AddFd(m_baseSocket.getFd());
    if (utils::CFdSetRetval::UNBLOCK == m_FdSet.Select()) {
        m_FdSet.RemoveFd(m_baseSocket.getFd());
        return CUdpDataLink(-1);
    }
    m_FdSet.RemoveFd(m_baseSocket.getFd());
    return CUdpDataLink(m_baseSocket.getFd());
}

//*****************************************************************************
// Method definitions "CUdpServer"

CUdpServer::CUdpServer(CBaseSocket&& rBaseSocket, unsigned int port) :
    m_pPrivate(std::make_unique<CUdpServerPrivate>(std::move(rBaseSocket),port))
{ }

CUdpServer::~CUdpServer() noexcept = default;

CUdpDataLink CUdpServer::waitForConnection()
{
    return m_pPrivate->waitForConnection();
}
