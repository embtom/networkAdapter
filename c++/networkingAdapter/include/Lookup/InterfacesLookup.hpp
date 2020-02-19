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

#ifndef _INTERFACESLOOKUP_H_
#define _INTERFACESLOOKUP_H_

#include <map>
#include <string>
#include <memory>
#include <enum_reflect.hpp>
#include <IpAddress.hpp>

DECLARE_ENUM(EIfState, unsigned, down, up, running);

namespace EtNet
{

class CNetInterfacePrivat;
class CNetInterface
{
public:
    using IfMap = std::map<unsigned int, CNetInterface>;

    CNetInterface(const CNetInterface&) noexcept            = delete;
    CNetInterface& operator=(const CNetInterface&) noexcept = delete;
    CNetInterface(CNetInterface&&) noexcept                 = default;
    CNetInterface& operator=(CNetInterface&&) noexcept      = default;
    ~CNetInterface();

    unsigned getIfIndex() const noexcept;
    std::string getName() const noexcept;
    CIpAddress::IpAddresses getAddresses() const noexcept;
    CIpAddress::IpAddresses getSubMask() const noexcept;
    CIpAddress::IpAddresses getBroadcast() const noexcept;
    EIfState getState() const noexcept;
    unsigned getMtu() const noexcept;

    static IfMap getInterfaceMap(bool bUpOnly) noexcept;

private:
    CNetInterface(unsigned int index, std::string&& name );
    std::unique_ptr<CNetInterfacePrivat> m_private;
};

} //EtNet

#endif //_INTERFACESLOOKUP_H_