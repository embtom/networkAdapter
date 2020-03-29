
#include <iostream>
#include <algorithm>
#include <array>
#include <string>
#include <map>
#include <docopt.h>
#include <BaseSocket.hpp>
#include <Udp/UdpClient.hpp>

#define PORT_NUM 50002


#define BUF_SIZE 100

int main(int argc, char *argv[])
{
    constexpr std::string_view docOptCmd =
    R"(EXA_UdpClient.
        Usage:
        EXA_UdpClient (-c | --connect) <hostname>
        EXA_UdpClient (-h | --help)
        EXA_UdpClient --version
        Options:
        -h --help     Show this screen.
        --version     Show version.
    )";

    constexpr auto networkAdapterVersion = "networkAdapter " NETWORKING_ADAPTER_VERSION;
    using ArgMap_t = std::map<std::string, docopt::value>;
    ArgMap_t args = docopt::docopt(std::string(docOptCmd),
                                   { argv + 1, argv + argc },
                                   true,
                                   networkAdapterVersion);
    if (!(args["<hostname>"])) {
        return 0;
    }

    char buffer [128];
    utils::span<char> rxSpan(buffer);

    /* Create a datagram socket; send to an address in the IPv6 domain */
    auto baseSocket = EtNet::CBaseSocket(EtNet::CBaseSocket::SoReuseSocket(EtNet::ESocketMode::INET_DGRAM));
    EtNet::CUdpClient abc (std::move(baseSocket));

    std::string hostName(args["<hostname>"].asString());
    std::cout << "try to connect to: " << hostName << std::endl;

    EtNet::CUdpDataLink d;
    try {
        d = abc.getLink(hostName, PORT_NUM);
    }
    catch(const std::exception& e) {
        std::cout << "Failed to establish connection: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    /* Send messages to server; echo responses on stdout */
    for (int j = 0; j < 2; j++) {

        std::string test = std::string("Hallo") + std::to_string(j);
        d.send(utils::span<char>(test));

        d.reciveFrom(rxSpan, [](EtNet::SPeerAddr ClientAddr, utils::span<char> rx) {
            std::cout << "Rcv Len: " << rx.data() << " Size: " << rx.size() << std::endl;
            return true;
        });
    }

    return EXIT_SUCCESS;
}