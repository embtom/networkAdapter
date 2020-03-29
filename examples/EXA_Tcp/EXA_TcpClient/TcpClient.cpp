
#include <iostream>
#include <algorithm>
#include <array>
#include <string>
#include <map>
#include <docopt.h>
#include <span.h>
#include <BaseSocket.hpp>
#include <Tcp/TcpClient.hpp>
#include <Tcp/TcpDataLink.hpp>

int main(int argc, char *argv[])
{
    constexpr std::string_view docOptCmd =
        R"(EXA_TcpClient.
            Usage:
            EXA_TcpClient (-c | --connect) <hostname>
            EXA_TcpClient (-h | --help)
            EXA_TcpClient --version
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

    auto baseSocket = EtNet::CBaseSocket(EtNet::ESocketMode::INET_STREAM);
    EtNet::CTcpClient TcpClient(std::move(baseSocket));

    std::string hostName(args["<hostname>"].asString());
    std::cout << "try to connect to: " << hostName << std::endl;

    EtNet::CTcpDataLink a;
    try {
        a = TcpClient.connect(hostName, 5001);
    }
    catch(const std::exception& e) {
        std::cout << "Failed to establish connection: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    char buffer [128];
    utils::span<char> rxSpan(buffer);
    std::string test ("Hallo");
    a.send(utils::span<char>(test));
    a.recive(rxSpan, [] (utils::span<char> rx) {
         std::cout << "Rcv Len: " << rx.data() << " Size: " << rx.size() << std::endl;;
         return true;
    });
}
