
#include <iostream>
#include <algorithm>
#include <IpAddress.hpp>
#include <string_view>
#include <docopt.h>
#include <Lookup/HostLookup.hpp>
#include <IpAddress.hpp>


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

