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

#ifndef _HOSTLOOKUP_H_
#define _HOSTLOOKUP_H_

//******************************************************************************
// Headers

#include <unordered_map> //std::unordered
#include <vector> //std::vector
#include <IpAddress.hpp> //CIpAddress

namespace EtNet
{

//*****************************************************************************
//! \brief CHostLookup
//!
class CHostLookup
{

public:
    using IpAddresses = std::vector<CIpAddress>;

    CHostLookup() noexcept                              = default;
    CHostLookup(const CHostLookup&)                     = default;
    CHostLookup& operator=(const CHostLookup&)          = default;
    CHostLookup(CHostLookup&&) noexcept                 = default;
    CHostLookup& operator=(CHostLookup&&) noexcept      = default;
    virtual ~CHostLookup() noexcept;

    CHostLookup(std::string&& rHostName);
    CHostLookup(const std::string& rHostName);
    CHostLookup(CIpAddress&& rIpAddress);
    CHostLookup(const CIpAddress& rIpAddress);

    IpAddresses addresses() const noexcept;
    std::string hostname() const noexcept;

private:
    std::string   m_hostName;
    IpAddresses   m_IpAddresses;

    static std::string requestHostname(const CIpAddress& rIpAddress);
    static IpAddresses requestIpAddresses(const std::string& rHostName);
};

} //EtNet

#endif //_HOSTLOOKUP_H_
