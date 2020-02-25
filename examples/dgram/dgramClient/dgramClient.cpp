
#include <iostream>
#include <BaseSocket.hpp>
#include <Dgram/DgramClient.hpp>
#include <span.h>

#define PORT_NUM 50002


#define BUF_SIZE 100

int main(int argc, char *argv[])
{
    int j;

    char buffer [128];
    utils::span<char> rxSpan(buffer);

    /* Create a datagram socket; send to an address in the IPv6 domain */
    auto baseSocket = EtNet::CBaseSocket(EtNet::CBaseSocket::SoReuseSocket(EtNet::ESocketMode::INET_DGRAM));
    EtNet::CDgramClient abc (std::move(baseSocket));
    EtNet::CDgramDataLink d = abc.getLink("localhost",PORT_NUM); //192.168.1.22

    /* Send messages to server; echo responses on stdout */
    for (j = 0; j < 2; j++) {

        std::string test = std::string("Hallo") + std::to_string(j);
        d.send(utils::span<char>(test));

        d.reciveFrom(rxSpan, [](EtNet::SPeerAddr ClientAddr, utils::span<char> rx) {
            std::cout << "Rcv Len: " << rx.data() << " Size: " << rx.size() << std::endl;
            return true;
        });
    }

    return EXIT_SUCCESS;
}