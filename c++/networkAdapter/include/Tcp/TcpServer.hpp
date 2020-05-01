/*
 * This file is part of the EMBTOM project
 * Copyright (c) 2018-2020 Thomas Willetal
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

#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_

//******************************************************************************
// Header

#include <tuple>
#include <memory>
#include <IpAddress.hpp>
#include <Tcp/TcpDataLink.hpp>

namespace EtNet
{

constexpr int maxConnectionBacklog = 5;

class CBaseSocket;
class CTcpServerPrivate;

//*****************************************************************************
//! \brief CTcpServer
//! TcpServer for stream connection
class CTcpServer
{
public:
    CTcpServer() noexcept                         = default;
    //Copy of server is delected can only be moved
    CTcpServer(const CTcpServer&)                 = delete;
    CTcpServer& operator= (const CTcpServer&)     = delete;
    CTcpServer(CTcpServer&&) noexcept             = default;
    CTcpServer& operator= (CTcpServer&&) noexcept = default;
    virtual ~CTcpServer() noexcept;

    //! Initialize TcpServer with a BaseSocket and port to listen
    CTcpServer(CBaseSocket&& rBaseSocket, unsigned int port);

    //! wait for connecton, if the routine unblocks a client have connected
    //! and returns the IpAddress of the Client and a TcpDataLink Object.
    //! It is used to communicate the client
    std::tuple<CTcpDataLink, CIpAddress> waitForConnection();
private:
    std::unique_ptr<CTcpServerPrivate> m_pPrivate;
};

} //EtNet

#endif // _TCPSERVER_H_
