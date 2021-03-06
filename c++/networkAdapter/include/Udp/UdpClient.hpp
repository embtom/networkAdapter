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

#ifndef _UDPCLIENT_H_
#define _UDPCLIENT_H_

//******************************************************************************
// Header

#include <tuple>
#include <string>
#include <memory>
#include <Udp/UdpDataLink.hpp>

namespace EtNet
{

class CBaseSocket;
class CUdpClientPrivate;

//*****************************************************************************
//! \brief CUdpClient
//! TcpClient for dgram connection
class CUdpClient
{
public:
    CUdpClient() noexcept                           = default;
    CUdpClient(CUdpClient&&) noexcept             = default;
    CUdpClient& operator= (CUdpClient&&) noexcept = default;
    CUdpClient(const CUdpClient&)                 = delete;
    CUdpClient& operator= (const CUdpClient&)     = delete;
    virtual ~CUdpClient() noexcept;

    //! Initialize TcpClient by a BaseSocket
    CUdpClient(CBaseSocket &&rBaseSocket);

    //! Request a link to send Udp packets to a UdpServer
    //! The returned UdpDataLink Object is used for communication
    CUdpDataLink getLink(const std::string& rHost, unsigned int port);

private:
    std::unique_ptr<CUdpClientPrivate> m_pPrivate;
};

} // EtNet
#endif // _UDPCLIENT_H_
