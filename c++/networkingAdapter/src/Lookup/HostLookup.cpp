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

#include <Lookup/HostLookup.hpp>
#include <iostream> //std::cout
#include <string> //std::string
#include <netdb.h> //getaddrinfo
#include <arpa/inet.h> //inet_ntop
#include <sys/socket.h> //AF_INET AF_INET6
#include <error_msg.hpp> //utils::buildErrorMessage
#include <make_unordered_map.h> //utils::make_unordered_map

using namespace EtNet;

CHostLookup::CHostLookup(std::string&& rHostName) :
    CHostLookup(rHostName)
{ }

CHostLookup::CHostLookup(const std::string& rHostName) :
   m_hostName(rHostName),
   m_IpAddresses(requestIpAddresses(rHostName))
{

}

CHostLookup::CHostLookup(const CIpAddress& rIpAddress) :
    m_hostName(requestHostname(rIpAddress)),
    m_IpAddresses(requestIpAddresses(m_hostName))
{ }

CHostLookup::CHostLookup(CIpAddress&& rIpAddress) :
    CHostLookup(rIpAddress)
{ }

CHostLookup::IpAddresses CHostLookup::addresses() const noexcept
{
    return m_IpAddresses;
}

std::string CHostLookup::hostname() const noexcept
{
    return m_hostName;
}

CHostLookup::IpAddresses CHostLookup::requestIpAddresses(const std::string& rHostName)
{
    CHostLookup::IpAddresses hostIps;
    char addrstr[INET6_ADDRSTRLEN];

    struct addrinfo hints = {};
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags |= AI_CANONNAME;

    struct addrinfo *result, *rp;
    int ret = getaddrinfo (rHostName.c_str(), NULL, &hints, &result);
    if (ret != 0) {
         throw std::runtime_error(utils::buildErrorMessage("CHostLookup::", __func__, ": getaddrinfo: ", gai_strerror(ret)));
    }

    for(rp = result; rp != NULL; rp = rp->ai_next)
    {
        inet_ntop (result->ai_family, result->ai_addr->sa_data, addrstr, sizeof(addrstr));

        switch (result->ai_family)
        {
            case AF_INET:
            {
                sockaddr_in *pSockAddr = reinterpret_cast<sockaddr_in*>(result->ai_addr);
                hostIps.emplace_back(CIpAddress(pSockAddr->sin_addr));

                break;
            }
            case AF_INET6:
            {
                sockaddr_in6 *pSockAddr = reinterpret_cast<sockaddr_in6*>(result->ai_addr);
                hostIps.emplace_back(CIpAddress(pSockAddr->sin6_addr));
                break;
            }
        }
        result = result->ai_next;
    }
    freeaddrinfo(result);
    return hostIps;
}

std::string CHostLookup::requestHostname(const CIpAddress& rIpAddress)
{
    std::string hostName;
    auto requestHostname = [&hostName] (const sockaddr* sa, socklen_t saLen)
    {
        char name[NI_MAXHOST] = {0};
        int ret = getnameinfo((sockaddr*)sa, saLen, name, NI_MAXHOST, nullptr, 0, NI_NAMEREQD | NI_NOFQDN);
        if (ret != 0) {
            throw std::runtime_error(utils::buildErrorMessage("CHostLookup::", __func__, ": getnameinfo: ", gai_strerror(ret)));
        }
        hostName = std::string(name);
    };

    if(rIpAddress.is_v4())
    {
        sockaddr_in ip4;
        ip4.sin_family = AF_INET;
        std::memcpy(&ip4.sin_addr, rIpAddress.to_v4(), sizeof(in_addr));
        requestHostname((sockaddr*)&ip4, sizeof(sockaddr_in));
    }
    else if (rIpAddress.is_v6())
    {
        sockaddr_in6 ip6;
        ip6.sin6_family = AF_INET6;
        std::memcpy(&ip6.sin6_addr, rIpAddress.to_v6(), sizeof(in6_addr));
        requestHostname((sockaddr*)&ip6, sizeof(sockaddr_in6));
    }
    else {
        throw std::invalid_argument(utils::buildErrorMessage("CHostLookup::", __func__, ": unknown ipAddress type"));
    }

    return hostName;
}
