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

//******************************************************************************
// Header

#include <Tcp/TcpClient.hpp>

#include <iostream>
#include <stdexcept>
#include <algorithm>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>

#include <error_msg.hpp>
#include <Lookup/HostLookup.hpp>
#include <IpAddress.hpp>
#include <BaseSocket.hpp>

namespace EtNet
{

//*****************************************************************************
//! \brief CTcpClientPrivate
//!
class CTcpClientPrivate
{
public:
     CTcpClientPrivate(CBaseSocket&& rBaseSocket);
     CTcpDataLink connect(const std::string& rHost, unsigned int port);
private:
     CBaseSocket m_baseSocket;
};

}

using namespace EtNet;

//*****************************************************************************
// Method definitions "CTcpClientPrivate"
CTcpClientPrivate::CTcpClientPrivate(CBaseSocket&& rBaseSocket) :
    m_baseSocket(std::move(rBaseSocket))
{ }

CTcpDataLink CTcpClientPrivate::connect(const std::string& rHost, unsigned int port)
{
    CHostLookup::IpAddresses ipList;
    try  { ipList = CHostLookup(CIpAddress(rHost)).addresses(); }  catch(...) {  }

    if (ipList.empty())
    {
        ipList = CHostLookup(rHost).addresses();
    }

    int domain = m_baseSocket.getDomain();
    auto it = std::find_if(ipList.begin(), ipList.end(),[&domain] (const auto &elm)
    {
        if (elm.is_v4() && (domain == AF_INET)) {
            return true;
        }
        else if (elm.is_v6() && (domain == AF_INET6)) {
            return true;
        }
        else {
            return false;
        }
    });

    if (it == ipList.end()) {
        throw std::runtime_error(utils::buildErrorMessage("CTcpClient: ", __func__, " : No valid Ip available"));
    }

    if ((*it).is_v4())
    {
        sockaddr_in serverAddr{};
        serverAddr.sin_family       = AF_INET;
        serverAddr.sin_port         = htons(port);
        std::memcpy(&serverAddr.sin_addr, (*it).to_v4(), sizeof(in_addr));

        if (::connect(m_baseSocket.getFd(), (sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
            throw std::runtime_error(utils::buildErrorMessage("ConnectSocket::", __func__, ": connect: ", strerror(errno)));
        }
    }
    else if((*it).is_v6())
    {
        sockaddr_in6 serverAddr{};
        serverAddr.sin6_family       = AF_INET6;
        serverAddr.sin6_port         = htons(port);
        std::memcpy(&serverAddr.sin6_addr, (*it).to_v6(), sizeof(in6_addr));

        if (::connect(m_baseSocket.getFd(), (sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
            throw std::runtime_error(utils::buildErrorMessage("ConnectSocket::", __func__, ": connect: ", strerror(errno)));
        }
    }
    else {
        throw std::logic_error(utils::buildErrorMessage("ConnectSocket::", __func__, " : No valid Ip to connect"));
    }

    return CTcpDataLink(std::move(m_baseSocket));
}

//*****************************************************************************
// Method definitions "CTcpClient"

CTcpClient::CTcpClient(CBaseSocket&& rBaseSocket) :
    m_pPrivate(new CTcpClientPrivate(std::move(rBaseSocket)))
{ }

CTcpClient::~CTcpClient() noexcept = default;

CTcpDataLink CTcpClient::connect(const std::string& rHost, unsigned int port)
{
     return m_pPrivate->connect(rHost,port);
}
