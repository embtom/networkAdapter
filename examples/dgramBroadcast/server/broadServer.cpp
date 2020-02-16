
#include <iostream>
#include <thread>
#include <span.h>
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

    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer); 
        } else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer); 
        } 
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);



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

