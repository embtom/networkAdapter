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

#ifndef _IPADDRESS_H_
#define _IPADDRESS_H_

//******************************************************************************
// Headers

#include <variant> //std::variant
#include <vector>
#include <type_traits> //enable_if, remove_reference
#include <string> //std::string
#include <cstring> //std::memcmp
#include <netinet/in.h> //in_addr, in6_addr

#undef min
#undef max

namespace EtNet
{

enum class EAddressFamily{
    INET,
    INET6,
    INV
};

//*****************************************************************************
//! \brief CIpAddress
//!It is a representation of IP address, suitable IPv4 or the IPv6
//!address.
//!
//!IPv6 addresses are supported only if the target platform supports IPv6.
class CIpAddress
{
public:
    using IpAddresses = std::vector<CIpAddress>;

    //!Creates a (zero) Ip adddress
    constexpr CIpAddress() noexcept         = default;

    CIpAddress(const CIpAddress&)            = default;
    CIpAddress& operator=(const CIpAddress&) = default;
    CIpAddress(CIpAddress&&)                 = default;
    CIpAddress& operator=(CIpAddress&&)      = default;
    virtual ~CIpAddress() noexcept           = default;

    //! Creates an IPAddress from a native inet address type.
    //! Depending on the passed address type (in_addr or in6_addr)
    //! the containing address family is selected.
    template <typename T,
              typename std::enable_if_t<std::is_same<std::remove_reference_t<T>, in_addr>::value ||
                                        std::is_same<std::remove_reference_t<T>, in6_addr>::value, int> = 0>
    constexpr CIpAddress(T&& rIpAddr) noexcept :
        m_address(std::forward<T>(rIpAddr))
    { }

    //! Creates an IPAddress a string in representation format of dotted decimal
	//! for IPv4 or hex string for IPv6
    //! e.g. "x.x.x.x" for IPv4,
    //! e.g. "x:x:x:x:x:x:x:x" for Ipv6
    CIpAddress(const std::string& rIpStr);
    CIpAddress(std::string&& rIpStr) :
        CIpAddress(rIpStr)
    { }

    //! return the the contained ip address as string
    std::string toString() const noexcept;

    //! return ture if containing address is Ipv4
    bool is_v4() const noexcept;

    //! return ture if containing address is Ipv6
    bool is_v6() const noexcept;

    //! return ture if containing address is loopback address
    bool is_loopback() const noexcept;

    //! return ture if containing address is a broadcast address
    bool is_broadcast() const noexcept;

    //! return ture if containing address is a submask
    bool is_submask() const noexcept;

    //! return ture if the IpAddress is not initialized
    bool empty() const noexcept;

    //! Request of containing address family
    EAddressFamily addressFamily() const noexcept;

    //! returns the internal IPv4 address structure
    //! otherwise nullptr will be returned
    const in_addr* to_v4() const noexcept;

    //! returns the internal IPv6 address structure
    //! otherwise nullptr will be returned
    const in6_addr* to_v6() const noexcept;

    //! return of the boradcast address derived form the passed
    //! Network SubMask
    CIpAddress broadcast(const CIpAddress& rSubmask) const noexcept;

    bool operator== (const CIpAddress& rhs) const noexcept;
    bool operator!= (const CIpAddress& rhs) const noexcept;

private:
    static size_t charCount(const std::string& rIpStr, char toCount) noexcept;
    mutable std::variant<std::monostate, in6_addr, in_addr> m_address;
};

} //EtNet

#endif // _IPADDRESS_H_
