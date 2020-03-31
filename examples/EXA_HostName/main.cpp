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

//******************************************************************************
// Headers

#include <iostream>
#include <algorithm>
#include <IpAddress.hpp>
#include <string_view>
#include <docopt.h>
#include <Lookup/HostLookup.hpp>
#include <IpAddress.hpp>

//*****************************************************************************
//! \brief EXA_HostName
//!

int main(int argc, const char** argv)
{
    constexpr std::string_view docOptCmd =
        R"(EXA_HostName.
            Usage:
            EXA_HostName --host <name>
            EXA_HostName (-h | --help)
            EXA_HostName --version
            Options:
            -h --help     Show this screen.
            --version     Show version.
        )";

    constexpr auto networkAdapterVersion = "networkAdapter " NETWORKING_ADAPTER_VERSION;
    using ArgMap_t = std::map<std::string, docopt::value>;
    ArgMap_t args = docopt::docopt(std::string(docOptCmd),
                                   { argv + 1, argv + argc },
                                   true,
                                   networkAdapterVersion);

    if (!(args["<name>"])) {
        return 0;
    }

    EtNet::CHostLookup hostLookup (std::string(args["<name>"].asString()));
    auto hostAddresses = hostLookup.addresses();

    EtNet::CIpAddress::IpAddresses Ipv4;
    EtNet::CIpAddress::IpAddresses Ipv6;
    std::copy_if(hostAddresses.begin(), hostAddresses.end(), std::back_inserter(Ipv4), [] (EtNet::CIpAddress& ip)
    {
        if (ip.is_v4())
            return true;
        else
            return false;
    });

    std::copy_if(hostAddresses.begin(), hostAddresses.end(), std::back_inserter(Ipv6), [] (EtNet::CIpAddress& ip)
    {
        if (ip.is_v6())
            return true;
        else
            return false;
    });

    std::cout << "IPv4 Addresses:" << std::endl;
    for(const EtNet::CIpAddress& address : Ipv4)
    {
        std::cout << address.toString() << std::endl;
    }

    std::cout << "IPv6 Addresses:" << std::endl;
    for(const EtNet::CIpAddress& address : Ipv6)
    {
        std::cout << address.toString() << std::endl;
    }

    return 0;
}

