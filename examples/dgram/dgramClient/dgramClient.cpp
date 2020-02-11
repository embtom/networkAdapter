
#include <iostream>
#include <BaseSocket.hpp>
#include <dgram/DgramClient.hpp>


#define PORT_NUM 50002


int main(int argc, char *argv[])
{
    uint8_t buffer [128];

    auto baseSocket = EtNet::CBaseSocket(EtNet::ESocketMode::INET_DGRAM);
    EtNet::CDgramClient dgramClient(std::move(baseSocket));

    auto[a, b] = dgramClient.getLink("localhost",PORT_NUM);

    for (int i = 0 ; i < 2; i++)
    {
        std::string test = std::string("Hallo") + std::to_string(i);
        a.send(test.c_str(), test.length());
            int rcvLen= a.recive(buffer, sizeof(buffer), [](std::size_t len) {
            std::cout << "Rcv Len:" << len << std::endl;;
            return true;
        });
        std::cout << "Rcv: " << (char*)buffer << std::endl;
    }
}
