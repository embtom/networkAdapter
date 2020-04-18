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
// Headers

#include <iostream>
#include <IpAddress.hpp>
#include <Lookup/InterfacesLookup.hpp>

//*****************************************************************************
//! \brief EXA_InterfaceLookup
//!

int main ()
{
    auto interfaces = EtNet::CNetInterface::getStateMap(true);
    std::cout << "Network Interfaces overview:" << std::endl;
    for (auto& it : interfaces)
    {
        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << it.second.getIfIndex() << " " << it.second.getName() << " state: " << it.second.getState() << " " << it.second.getMtu() << std::endl;
        std::cout << "IpAddresses:" << std::endl;
        for (auto& ip : it.second.getAddresses()) {
            if (ip.is_loopback()) {
                std::cout<< "loopbackIp is: " << ip.toString() << std::endl;
            }
            else {
                std::cout << ip.toString() << std::endl;
            }
        }
        for (auto& mask : it.second.getSubMask()) {
            if (mask.is_submask()) {
                std::cout << "Submask is " << mask.toString() << std::endl;

            }
        }
        for (auto& broadcast : it.second.getBroadcast()) {
            if (broadcast.is_broadcast()) {
                std::cout << "broadcast is: " << std::endl;
            }
            std::cout << broadcast.toString() << std::endl;
        }
    }

    std::cout << "\n All local Ipv4 addresses:" << std::endl;

    for(auto& a : EtNet::CNetInterface::getAllIpv4Ip(false))
    {
        std::cout << "Ipv4 :" << a.toString() << std::endl;
    }

    std::cout << "\n All local Ipv6 addresses:" << std::endl;

    for(auto& a : EtNet::CNetInterface::getAllIpv6Ip(false))
    {
        std::cout << "Ipv6 :" << a.toString() << std::endl;
    }

    std::cout << "\n All local Ipv4 Submask:" << std::endl;

    for(auto& a : EtNet::CNetInterface::getAllIpv4Submask())
    {
        std::cout << "Ipv6 Submask:" << a.toString() << std::endl;
    }
}

