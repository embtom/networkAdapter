
#include <iostream>
#include <BaseSocket.hpp>
#include <dgram/DgramClient.hpp>
#include <span.h>

#define PORT_NUM 50002


int main(int argc, char *argv[])
{
    char buffer [128];
    utils::span<char> rxSpan(buffer);

    auto baseSocket = EtNet::CBaseSocket(EtNet::ESocketMode::INET_DGRAM);
    EtNet::CDgramClient dgramClient(std::move(baseSocket));

    auto[a, b] = dgramClient.getLink("localhost",PORT_NUM);

    for (int i = 0 ; i < 2; i++)
    {
        std::string test = std::string("Hallo") + std::to_string(i);
        a.send(utils::span<char>(test));
            a.recive(rxSpan, [](utils::span<char> rx) {
            std::cout << "Rcv Len: " << rx.data() << " Size: " << rx.size() << std::endl;
            return true;
        });
    }
}
