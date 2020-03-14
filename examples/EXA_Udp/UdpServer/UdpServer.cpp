
#include <iostream>
#include <Udp/UdpServer.hpp>
#include <thread>
#include <span.h>


 #define PORT_NUM 50002

int main()
{
    auto baseSocket = EtNet::CBaseSocket(EtNet::ESocketMode::INET_DGRAM);
    EtNet::CUdpServer UdpServer(std::move(baseSocket), PORT_NUM);

    while (true)
    {
        char buffer [128] = {0};
        utils::span<char> rxtxSpan (buffer);

        EtNet::CUdpDataLink a = UdpServer.waitForConnection();
        a.reciveFrom(rxtxSpan,[&a, &UdpServer](EtNet::SPeerAddr ClientAddr, utils::span<char> rx)
        {
            std::cout << "Message form: " << ClientAddr.Ip.toString() << " with length: " << rx.size() << std::endl;

            for(auto &elem : rx) {
                elem = toupper(elem);
            }

            a.sendTo(ClientAddr, rx);
            return true;
        });

    }
}

