
#include <iostream>
#include <IpAddress.hpp>
#include <Lookup/InterfacesLookup.hpp>

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

