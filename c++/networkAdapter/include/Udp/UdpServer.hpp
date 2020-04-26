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

#ifndef _UDPSERVER_H_
#define _UDPSERVER_H_

//******************************************************************************
// Header

#include <tuple>
#include <memory>
#include <IpAddress.hpp>
#include <Udp/UdpDataLink.hpp>

namespace EtNet
{

class CBaseSocket;
class CUdpServerPrivate;

//*****************************************************************************
//! \brief CUdpServer
//!
class CUdpServer
{
public:
    CUdpServer(CBaseSocket&& rBaseSocket, unsigned int port);

    CUdpServer(const CUdpServer&)                 = delete;
    CUdpServer& operator= (const CUdpServer&)     = delete;
    CUdpServer(CUdpServer&&) noexcept             = default;
    CUdpServer& operator= (CUdpServer&&) noexcept = default;
    CUdpServer() noexcept                           = default;
    ~CUdpServer() noexcept;

    CUdpDataLink waitForConnection();

private:
    std::unique_ptr<CUdpServerPrivate> m_pPrivate;
};

} // EtNet

#endif // _UDPSERVER_H_