
#include <iostream>
#include <dgram/DgramServer.hpp>
#include <thread>
#include <span.h>


#define PORT_NUM 50002

int main()
{
    auto baseSocket = EtNet::CBaseSocket(EtNet::ESocketMode::INET_DGRAM);
    EtNet::CDgramServer DgramServer(std::move(baseSocket), PORT_NUM);

    while (true)
    {
        char buffer [128] = {0};
        utils::span<char> rxtxSpan (buffer);

        EtNet::SPeerAddr addr;

        auto [a] = DgramServer.waitForConnection();
        a.reciveFrom(rxtxSpan,[&addr, &buffer, &DgramServer](EtNet::SPeerAddr ClientAddr, utils::span<char> rx)
        {
            addr = std::move(ClientAddr);
            std::cout << "Message form: " << addr.Ip.toString() << " with length: " << rx.size() << std::endl;

            for (int j = 0; j < rx.size(); j++) {
                buffer[j] = toupper((unsigned char) buffer[j]);
            }
            return true;
        });

        a.sendTo(addr, rxtxSpan);
    }
}

