
#include <iostream>
#include <BaseSocket.hpp>
#include <Lookup/InterfacesLookup.hpp>
#include <Dgram/DgramClient.hpp>
#include <span.h>
#include <thread>


#define PORT_NUM 50002

using namespace EtNet;

int main(int argc, char *argv[])
{
    char buffer [128];
    utils::span<char> rxSpan(buffer);


    CIpAddress::IpAddresses broadIp = CNetInterface::getAllIpv4Broadcast();

    std::cout << "Broadcast Address to send is: " << broadIp.at(0).toString() << std::endl;

    auto baseSocket = CBaseSocket::SoBroadcast(
                      CBaseSocket::SoReuseSocket(
                      CBaseSocket(ESocketMode::INET_DGRAM)));

    EtNet::CDgramClient dgramClient(std::move(baseSocket));

    CDgramDataLink a = dgramClient.getLink(broadIp.at(0).toString(), PORT_NUM);

    std::thread thd ([b = a]()
    {
        while (true)
        {
            char buffer [128];
            utils::span<char> rxSpan(buffer);
            b.reciveFrom(rxSpan, [](SPeerAddr peer, utils::span<char> rx) {
                std::cout << "RCV from " << peer.Ip.toString() << std::endl;
                std::cout << "Rcv Len: " << rx.data() << " Size: " << rx.size() << std::endl;
                return true;
            });
        }
    });

    for (int i = 0 ; i < 2; i++)
    {
        std::string test = std::string("Hallo") + std::to_string(i);
        a.send(utils::span<char>(test));
        // a.reciveFrom(rxSpan, [](SPeerAddr peer, utils::span<char> rx) {
        //     std::cout << "RCV from " << peer.Ip.toString() << std::endl;
        //     std::cout << "Rcv Len: " << rx.data() << " Size: " << rx.size() << std::endl;
        //     return true;
        // });
        // a.reciveFrom(rxSpan, [](SPeerAddr peer, utils::span<char> rx) {
        //     std::cout << "RCV from " << peer.Ip.toString() << std::endl;
        //     std::cout << "Rcv Len: " << rx.data() << " Size: " << rx.size() << std::endl;
        //     return true;
        // });
    }
    thd.join();
}
