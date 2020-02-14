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

#ifndef _HOSTNAME_H_
#define _HOSTNAME_H_

#include <unordered_map> //std::unordered
#include <vector> //std::vector
#include <IpAddress.hpp> //CIpAddress


namespace EtNet
{

class CHostLookup
{

public:
    using IpAddresses = std::vector<CIpAddress>;
    CHostLookup(std::string&& rHostName);
    CHostLookup(const std::string& rHostName);
    CHostLookup(CIpAddress&& rIpAddress);
    CHostLookup(const CIpAddress& rIpAddress);

    CHostLookup()                              = default;
    CHostLookup(const CHostLookup&)            = default;
    CHostLookup& operator=(const CHostLookup&) = default;
    CHostLookup(CHostLookup&&)                 = default;
    CHostLookup& operator=(CHostLookup&&)      = default;

    IpAddresses addresses() const noexcept;
    std::string hostname() const noexcept;

private:
    std::string   m_hostName;
    IpAddresses   m_IpAddresses;

    static std::string requestHostname(const CIpAddress& rIpAddress);
    static IpAddresses requestIpAddresses(const std::string& rHostName);
};

} //EtNet

#endif // _HOSTNAME_H_
