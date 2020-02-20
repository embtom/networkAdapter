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
//!
class CIpAddress
{
public:
    using IpAddresses = std::vector<CIpAddress>;

    constexpr CIpAddress() noexcept         = default;
    CIpAddress(const CIpAddress&)            = default;
    CIpAddress& operator=(const CIpAddress&) = default;
    CIpAddress(CIpAddress&&)                 = default;
    CIpAddress& operator=(CIpAddress&&)      = default;

    template <typename T,
              typename std::enable_if_t<std::is_same<std::remove_reference_t<T>, in_addr>::value ||
                                        std::is_same<std::remove_reference_t<T>, in6_addr>::value, int> = 0>
    constexpr CIpAddress(T&& rIpAddr) noexcept :
        m_address(std::forward<T>(rIpAddr))
    { }

    CIpAddress(const std::string& rIpStr);
    CIpAddress(std::string&& rIpStr) :
        CIpAddress(rIpStr)
    { }

    std::string toString() const noexcept;
    bool is_v4() const noexcept;
    bool is_v6() const noexcept;
    bool is_loopback() const noexcept;
    bool is_broadcast() const noexcept;
    bool is_submask() const noexcept;

    bool empty() const noexcept;

    EAddressFamily addressFamily() const noexcept;

    const in_addr* to_v4() const noexcept;
    const in6_addr* to_v6() const noexcept;

    CIpAddress broadcast(const CIpAddress& rSubmask) const noexcept;

    bool operator== (const CIpAddress& rhs) const noexcept;
    bool operator!= (const CIpAddress& rhs) const noexcept;
private:

    static size_t charCount(const std::string& rIpStr, char toCount) noexcept;
    mutable std::variant<std::monostate, in6_addr, in_addr> m_address;
};

} //EtNet

#endif // _IPADDRESS_H_
