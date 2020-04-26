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

#include <iostream>
#include <algorithm>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <error_msg.hpp>
#include <IpAddress.hpp>

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

//*****************************************************************************
// Method definitions "CIpAddress"

EtNet::CIpAddress::CIpAddress(const std::string& rIpStr)
{
    size_t cnt = charCount(rIpStr, '.');
    if(cnt == 3)
    {
        in_addr IpAddr = {0};
        if(inet_pton(AF_INET, rIpStr.c_str(), &IpAddr))
        {
            m_address.emplace<in_addr>(IpAddr);
            return;
        }
    }
    cnt = charCount(rIpStr, ':');
    if (cnt)
    {
        in6_addr IpAddr = {0};
        if(inet_pton(AF_INET6, rIpStr.c_str(), &IpAddr))
        {
            m_address.emplace<in6_addr>(IpAddr);
            return;
        }
    }
    throw std::runtime_error(utils::buildErrorMessage("CIpAddress::", __func__," No valid Ip string"));
}

std::string EtNet::CIpAddress::toString() const noexcept
{
    std::string ret;
    std::visit(overload{
        [&ret](const in_addr& val)
        {
            char addrstr[INET_ADDRSTRLEN] = {0};
            inet_ntop(AF_INET, &val, addrstr, sizeof(addrstr));
            ret = std::string(addrstr);
        },
        [&ret](const in6_addr& val)
        {
            char addrstr[INET6_ADDRSTRLEN] = {0};
            inet_ntop(AF_INET6, &val, addrstr, sizeof(addrstr));
            ret = std::string(addrstr);
        },
        [&ret](const std::monostate& )
        {
            ret = std::string();
        }
    }, m_address);

    return ret;
}

bool EtNet::CIpAddress::is_v4() const noexcept
{
    bool ret;
    std::visit(overload{
        [&ret](const in_addr& )        { ret = true; },
        [&ret](const in6_addr& )       { ret = false; },
        [&ret](const std::monostate& ) { ret = false; }
    }, m_address);
    return ret;
}

bool EtNet::CIpAddress::is_v6() const noexcept
{
    bool ret;
    std::visit(overload{
        [&ret](const in_addr& )        { ret = false; },
        [&ret](const in6_addr& )       { ret = true; },
        [&ret](const std::monostate& ) { ret = false; }
    }, m_address);
    return ret;
}

bool EtNet::CIpAddress::is_loopback() const noexcept
{
    bool ret;
    std::visit(overload{
        [&ret](const in_addr& addr)
        {
	        ret =(ntohl(addr.s_addr) & 0xFF000000) == 0x7F000000;
        },
        [&ret](const in6_addr& addr)
        {
            if ((ntohl(addr.__in6_u.__u6_addr32[3]) == 0x00000001) &&
                (addr.__in6_u.__u6_addr32[2] == 0x0000) &&
                (addr.__in6_u.__u6_addr32[1] == 0x0000) &&
                (addr.__in6_u.__u6_addr32[0] == 0x0000)) {
                ret = true;
            }
            else {
                ret = false;
            }
        },
        [&ret](const std::monostate& ) { ret = false; }
    }, m_address);
    return ret;
}

bool EtNet::CIpAddress::is_submask() const noexcept
{
    bool ret;
    std::visit(overload{
        [&ret](const in_addr& addr)
        {
	        ret =(ntohl(addr.s_addr) & 0xFF000000) == 0xFF000000;
        },
        [&ret](const in6_addr& addr)
        {
            ret = (addr.__in6_u.__u6_addr32[0] == 0xffff);
        },
        [&ret](const std::monostate& ) { ret = false; }
    }, m_address);
    return ret;
}

bool EtNet::CIpAddress::is_broadcast() const noexcept
{
    bool ret;
    std::visit(overload{
        [&ret](const in_addr& addr)
        {
	        ret = (ntohl(addr.s_addr) & 0x000000FF) == 0x000000FF;
        },
        [&ret](const in6_addr& )       { ret = false; },
        [&ret](const std::monostate& ) { ret = false; }
    }, m_address);

    return ret;
}

bool EtNet::CIpAddress::empty() const noexcept
{
    bool ret;
    std::visit(overload{
        [&ret](const in_addr& )        { ret = false; },
        [&ret](const in6_addr& )       { ret = false; },
        [&ret](const std::monostate& ) { ret = true; }
    }, m_address);
    return ret;
}

EtNet::EAddressFamily EtNet::CIpAddress::addressFamily() const noexcept
{
    EtNet::EAddressFamily ret;
    std::visit(overload{
        [&ret](const in_addr& )        { ret = EtNet::EAddressFamily::INET; },
        [&ret](const in6_addr& )       { ret = EtNet::EAddressFamily::INET6; },
        [&ret](const std::monostate& ) { ret = EtNet::EAddressFamily::INV; }
    }, m_address);

    return ret;
}

const in_addr* EtNet::CIpAddress::to_v4() const noexcept
{
    if (!is_v4()) {
        return nullptr;
    }
    return std::get_if<in_addr>(&m_address);
}

const in6_addr* EtNet::CIpAddress::to_v6() const noexcept
{
    if (!is_v6()) {
        return nullptr;
    }
    return std::get_if<in6_addr>(&m_address);
}

size_t EtNet::CIpAddress::charCount(const std::string& rIpStr, char toCount) noexcept
{
    return std::count_if( rIpStr.begin(), rIpStr.end(), [toCount] ( char c )
    {
        if(c == toCount)
            return true;
        else
            return false;
    });
}

EtNet::CIpAddress EtNet::CIpAddress::broadcast(const EtNet::CIpAddress& rSubmask) const noexcept
{
    if(is_v4() != rSubmask.is_v4()) {
        return EtNet::CIpAddress();
    }

    if(is_v6() != rSubmask.is_v6()) {
        return EtNet::CIpAddress();
    }

    CIpAddress broadcastIp;
    std::visit(overload{
        [&broadcastIp, &rSubmask](const in_addr& val) {
            broadcastIp = CIpAddress(in_addr{(val.s_addr | ~(rSubmask.to_v4()->s_addr))});
        },
        [&broadcastIp, &rSubmask](const in6_addr& val) {
            ;
        },
        [](const std::monostate& ){ }
    }, m_address);

    return broadcastIp;
}

bool EtNet::CIpAddress::operator== (const EtNet::CIpAddress& rhs) const noexcept
{
    if(is_v4() != rhs.is_v4()) {
        return false;
    }

    if(is_v6() != rhs.is_v6()) {
        return false;
    }

    bool ret = false;

    std::visit(overload{
    [rhs, &ret](const in_addr& val)
    {
        ret = (std::memcmp(&val, rhs.to_v4(), sizeof(in_addr)) == 0);
    },
    [rhs, &ret](const in6_addr& val)
    {
        ret = (std::memcmp(&val, rhs.to_v6(), sizeof(in6_addr)) == 0);
    },
    [&ret](const std::monostate& )
    {
        ret = false;
    }
    }, m_address);

    return ret;
}

bool EtNet::CIpAddress::operator !=(const EtNet::CIpAddress& rhs) const noexcept
{
    return !((*this) == rhs);
}

