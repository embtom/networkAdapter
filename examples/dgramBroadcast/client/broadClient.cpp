
#include <iostream>
#include <BaseSocket.hpp>
#include <Lookup/InterfacesLookup.hpp>
#include <Dgram/DgramClient.hpp>
#include <threadLoop.h>
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

    unsigned rcvCount {0};
    CDgramDataLink a = dgramClient.getLink(broadIp.at(0).toString(), PORT_NUM);
    auto rcvFunc = [&a, &rcvCount]()
    {
        char buffer [128];
        utils::span<char> rxSpan(buffer);
        CDgramDataLink::ERet ret = a.reciveFrom(rxSpan, [&rcvCount](SPeerAddr peer, utils::span<char> rx) {
            std::cout << "RCV from " << peer.Ip.toString() << std::endl;
            std::cout << "Rcv Len: " << rx.data() << " Size: " << rx.size() << std::endl;
            rcvCount++;
            return true;
        });

        if (CDgramDataLink::ERet::UNBLOCK == ret) {
            return true;
        }

        if(rcvCount==2) {
            a.unblockRecive();
        }
        std::cout << "Called " << rcvCount << std::endl;
        return false;
    };
    utils::CThreadLoop rcvLoop (rcvFunc, "RX_Worker");
    rcvLoop.start(std::chrono::milliseconds::zero());

    for (int i = 0 ; i < 2; i++)
    {
        std::string test = std::string("Hallo") + std::to_string(i);
        a.send(utils::span<char>(test));
    }
    rcvLoop.waitUntilFinished();
}
