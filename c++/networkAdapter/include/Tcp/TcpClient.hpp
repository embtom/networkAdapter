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

#ifndef _TCPCLIENT_H_
#define _TCPCLIENT_H_

//******************************************************************************
// Header

#include <tuple>
#include <memory>
#include <string>
#include <Tcp/TcpDataLink.hpp>

namespace EtNet
{

class CBaseSocket;
class CTcpClientPrivate;

//*****************************************************************************
//! \brief CTcpClient
//! TcpClient for stream connection
class CTcpClient
{
public:
    CTcpClient() noexcept                         = default;
    CTcpClient(const CTcpClient&)                 = delete;
    CTcpClient& operator= (const CTcpClient&)     = delete;
    CTcpClient(CTcpClient&&) noexcept             = default;
    CTcpClient& operator= (CTcpClient&&) noexcept = default;
    virtual ~CTcpClient() noexcept;

    //! Initialize TcpClient by a BaseSocket
    CTcpClient(CBaseSocket &&rBaseSocket);

    //! Establish a connection to a TcpServer. The returned
    //! TcpDataLink Object is used for communication
    CTcpDataLink connect(const std::string& rHost, unsigned int port);
private:
    std::unique_ptr<CTcpClientPrivate> m_pPrivate;
};

} //EtNet
#endif // _TCPCLIENT_H_
