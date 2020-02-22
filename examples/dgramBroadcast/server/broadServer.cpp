
#include <iostream>
#include <thread>
#include <span.h>
#include <Lookup/InterfacesLookup.hpp>
#include <Lookup/HostLookup.hpp>
#include <Dgram/DgramServer.hpp>


#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT_NUM 50002

using namespace EtNet;

int main()
{
    auto loopup = std::move(EtNet::CHostLookup("localhost"));
    for(auto& elem: loopup.addresses()) {
        std::cout << "Addr: " << elem.toString() << std::endl;
    }


    auto a = CNetInterface::getStateMap(false);

    std::cout << "hallo" << std::endl;

    // auto baseSocket = std::move(CBaseSocket::SoReuseSocket(CBaseSocket(ESocketMode::INET_DGRAM)));

    // EtNet::CDgramServer DgramServer(std::move(baseSocket), PORT_NUM);

    // while (true)
    // {
    //     char buffer [128] = {0};
    //     utils::span<char> rxtxSpan (buffer);

    //     EtNet::CDgramDataLink a;
    //     std::tie(a) = DgramServer.waitForConnection();
    //     a.reciveFrom(rxtxSpan,[&a, &DgramServer](EtNet::SPeerAddr ClientAddr, utils::span<char> rx)
    //     {
    //         std::cout << "Message form: " << ClientAddr.Ip.toString() << " with length: " << rx.size() << std::endl;

    //         for(auto &elem : rx) {
    //             elem = toupper(elem);
    //         }

    //         a.sendTo(ClientAddr, rx);
    //         return true;
    //     });

    // }
}

