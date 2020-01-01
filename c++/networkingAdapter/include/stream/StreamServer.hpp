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

#ifndef _CSTREAMSERVER_H_
#define _CSTREAMSERVER_H_

#include <tuple>
#include <IpAddress.hpp>
#include <BaseSocket.hpp>
#include <stream/DataSocket.hpp>

namespace EtNet
{

constexpr int maxConnectionBacklog = 5;



class CStreamServer
{
public:
    CStreamServer(CBaseSocket&& rhs, int port);
    
    CStreamServer(const CStreamServer&)             = delete;
    CStreamServer& operator= (const CStreamServer&) = delete;
    CStreamServer(CStreamServer&&)                  = default;
    CStreamServer& operator= (CStreamServer&&)      = default;
    CStreamServer()                                 = default;
    
    std::tuple<CDataSocket, CIpAddress> waitForConnection();
private:
    CBaseSocket m_baseSocket;
};

} //EtNet
#endif // _CSTREAMSERVER_H_