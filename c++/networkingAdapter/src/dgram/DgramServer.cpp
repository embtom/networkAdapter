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

#include <dgram/DgramServer.hpp>
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
//! \brief CDgramClientPrivate
//!
class CDgramServerPrivate
{
public:
    CDgramServerPrivate(CBaseSocket&& rBaseSocket, int port);
    ~CDgramServerPrivate();
    void incomingConnectionCb(int fd) noexcept;
    std::tuple<CDgramDataLink> waitForConnection();

private:
    utils::CFdSet       m_FdSet;
    CBaseSocket         m_baseSocket;
    std::promise<int>   m_connectionPromise;
};

}

using namespace EtNet;

//*****************************************************************************
// Method definitions "CDgramServerPrivate"

CDgramServerPrivate::CDgramServerPrivate(CBaseSocket&& rBaseSocket, int port) :
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

    m_FdSet.AddFd(m_baseSocket.getFd(), [this] (int fd) {incomingConnectionCb(fd);});
}

CDgramServerPrivate::~CDgramServerPrivate()
{
    m_FdSet.RemoveFd(m_baseSocket.getFd());
    m_FdSet.UnBlock();
}

void CDgramServerPrivate::incomingConnectionCb(int fd) noexcept
{
    m_connectionPromise.set_value(fd);
}
    
std::tuple<CDgramDataLink> CDgramServerPrivate::waitForConnection()
{
    m_connectionPromise = std::promise<int>{};
    if (utils::CFdSetRetval::UNBLOCK == m_FdSet.Select()) {
        return CDgramDataLink(-1);
    }
    
    int fd =  m_connectionPromise.get_future().get();
    return std::tuple(CDgramDataLink(fd));
}

//*****************************************************************************
// Method definitions "CDgramServer"

void CDgramServer::privateDeleterHook(CDgramServerPrivate *it)
{
    delete it;
}   

CDgramServer::CDgramServer(CBaseSocket&& rBaseSocket, int port) :
    m_pPrivate(new CDgramServerPrivate(std::move(rBaseSocket),port))
{ }

CDgramServer::~CDgramServer() = default;


std::tuple<CDgramDataLink> CDgramServer::waitForConnection()
{
    return m_pPrivate->waitForConnection();
}
