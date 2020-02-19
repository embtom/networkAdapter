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

#include <iostream>
#include <tuple>
#include <type_traits>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <Lookup/InterfacesLookup.hpp>
#include <IpAddress.hpp>
#include <error_msg.hpp>

namespace EtNet
{
    struct CNetInterfacePrivat
    {
        CNetInterfacePrivat (unsigned int index, std::string&& name) noexcept;
        void requestMtu() noexcept;
        void addAddress(CIpAddress&& address) noexcept;
        void addMask(CIpAddress&& mask) noexcept;
        void addBroadcast(CIpAddress&& broadcast) noexcept;

        std::string m_name;
	    unsigned    m_index {0};
        CIpAddress::IpAddresses m_addressList;
        CIpAddress::IpAddresses m_maskList;
        CIpAddress::IpAddresses m_broadcastList;
    	bool        m_up {0};
	    bool        m_running {0};
	    unsigned    m_mtu {0};
    };
}

using namespace EtNet;

CNetInterfacePrivat::CNetInterfacePrivat (unsigned int index, std::string&& name) noexcept :
    m_name(std::move(name)), m_index(index)
{
    requestMtu();
}

void CNetInterfacePrivat::addAddress(CIpAddress&& address) noexcept
{
    if (address.empty()) {
        return;
    }
    if (std::find(m_addressList.begin(), m_addressList.end(), address) == m_addressList.end()) {
        m_addressList.emplace_back(address);
    }
}

void CNetInterfacePrivat::addMask(CIpAddress&& mask) noexcept
{
    if (mask.empty()) {
        return;
    }
    if (std::find(m_maskList.begin(), m_maskList.end(), mask) == m_maskList.end()) {
        m_maskList.emplace_back(mask);
    }
}

void CNetInterfacePrivat::addBroadcast(CIpAddress&& broadcast) noexcept
{
    if (broadcast.empty()) {
        return;
    }
    if (std::find(m_broadcastList.begin(), m_broadcastList.end(), broadcast) == m_broadcastList.end()) {
        m_broadcastList.emplace_back(broadcast);
    }
}

void CNetInterfacePrivat::requestMtu() noexcept
{
	struct ifreq ifr;
    int s;

	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return;

	ifr.ifr_addr.sa_family = AF_INET;
	strcpy(ifr.ifr_name, m_name.c_str());
	if (ioctl(s, SIOCGIFMTU, (caddr_t)&ifr) < 0) {
        return;
    }
	close(s);
    m_mtu = ifr.ifr_mtu;
}

CNetInterface::CNetInterface(unsigned int index, std::string&& name) :
    m_private(std::make_unique<CNetInterfacePrivat>(index,std::move(name)))
{ }

CNetInterface::~CNetInterface() = default;


unsigned CNetInterface::getIfIndex() const noexcept
{
    return m_private->m_index;
}

std::string CNetInterface::getName() const noexcept
{
    return m_private->m_name;
}

CIpAddress::IpAddresses CNetInterface::getAddresses() const noexcept
{
    return m_private->m_addressList;
}

CIpAddress::IpAddresses CNetInterface::getSubMask() const noexcept
{
    return m_private->m_maskList;
}

CIpAddress::IpAddresses CNetInterface::getBroadcast() const noexcept
{
    return m_private->m_broadcastList;
}

EIfState CNetInterface::getState() const noexcept
{
    if (m_private->m_running && m_private->m_up)
        return EIfState::running;
    else if (!m_private->m_running && m_private->m_up)
        return EIfState::up;
    else
        return EIfState::down;
}

unsigned CNetInterface::getMtu() const noexcept
{
    return m_private->m_mtu;
}

CNetInterface::IfMap CNetInterface::getInterfaceMap(bool OnlyRunning) noexcept
{
    ifaddrs *ifAddrStruct=NULL, *ifa=NULL;
    void * tmpAddrPtr=NULL;

    try
    {
        /* code */
        if (getifaddrs(&ifAddrStruct) == -1) {
            throw std::runtime_error(utils::buildErrorMessage("CNetInterface::", __func__, ": getifaddrs failure: ", strerror(errno)));
        }

        IfMap ifMap;
        IfMap::iterator ifIt;
        unsigned ifIndex;
        auto test_and_add = [&ifMap, &ifIt, &ifIndex](ifaddrs *ifa){
            ifIndex = if_nametoindex(ifa->ifa_name);
            ifIt = ifMap.find(ifIndex);
            if(ifIt == ifMap.end()) {
                bool valid;
                std::tie(ifIt,valid) = ifMap.emplace(std::piecewise_construct,
                                                    std::forward_as_tuple(ifIndex),
                                                    std::forward_as_tuple(CNetInterface(ifIndex,ifa->ifa_name)));
            }
        };

        for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
        {
            if (ifa->ifa_addr == NULL) {
                continue;
            }

            CIpAddress address, subnetMask, broadcastAddress;

            if ((OnlyRunning && !(ifa->ifa_flags & IFF_RUNNING))) {
                continue;
            }

            switch (ifa->ifa_addr->sa_family)
            {
                case AF_INET:
                {
                    test_and_add(ifa);
                    {
                        auto *pIpAddr = reinterpret_cast<sockaddr_in*>(ifa->ifa_addr);
                        address = CIpAddress (pIpAddr->sin_addr);
                    }
                    if(((ifa->ifa_flags & IFF_LOOPBACK) == 0) && (ifa->ifa_netmask != NULL))
                    {
                        auto *pMaskAddr = reinterpret_cast<sockaddr_in*>(ifa->ifa_netmask);
                        subnetMask = CIpAddress (pMaskAddr->sin_addr);
                    }
                    if((ifa->ifa_flags & IFF_BROADCAST) && (ifa->ifa_broadaddr != NULL))
                    {
                        auto *pBroadcastAddr = reinterpret_cast<sockaddr_in*>(ifa->ifa_broadaddr);
                        broadcastAddress = CIpAddress (pBroadcastAddr->sin_addr);
                    }
                    break;
                }
                case AF_INET6:
                {
                    test_and_add(ifa);
                    {
                        auto *pIpAddr = reinterpret_cast<sockaddr_in6*>(ifa->ifa_addr);
                        address = CIpAddress (pIpAddr->sin6_addr);
                    }
                    {
                        sockaddr_in6 *pMaskAddr = reinterpret_cast<sockaddr_in6*>(ifa->ifa_netmask);
                        subnetMask = CIpAddress (pMaskAddr->sin6_addr);
                    }
                    broadcastAddress = CIpAddress();

                    break;
                }
                default:
                {
                    ifIndex = 0;
                    break;
                }
            }

            if ((ifIndex > 0) &&
                ((OnlyRunning && (ifa->ifa_flags & IFF_RUNNING)) || !OnlyRunning))

            {
                ifIt->second.m_private->m_up = (ifa->ifa_flags && IFF_UP);
                ifIt->second.m_private->m_up = (ifa->ifa_flags && IFF_RUNNING);

                ifIt->second.m_private->addAddress(std::move(address));
                ifIt->second.m_private->addMask(std::move(subnetMask));
                ifIt->second.m_private->addBroadcast(std::move(broadcastAddress));
            }
        }

        if (ifAddrStruct != NULL) {
            freeifaddrs(ifAddrStruct);
        }
        return ifMap;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Failed to setup interface map with error" << e.what() << std::endl;
        return IfMap();
    }
}
