
#include <iostream>
#include <BaseSocket.hpp>
#include <Tcp/TcpClient.hpp>
#include <Tcp/TcpDataLink.hpp>
#include <algorithm>
#include <span.h>

int main(int argc, char *argv[])
{
    char buffer [128];
    utils::span<char> rxSpan(buffer);

    auto baseSocket = EtNet::CBaseSocket(EtNet::ESocketMode::INET_STREAM);
    EtNet::CTcpClient TcpClient(std::move(baseSocket));
    auto a = TcpClient.connect("tom-LIFEBOOK",5001);
//    auto a = TcpClient.connect("tom-fujitsu",5001);
//    auto a = TcpClient.connect("localhost",5001);

    std::string test ("Hallo");
    a.send(utils::span<char>(test));
    a.recive(rxSpan, [] (utils::span<char> rx) {
         std::cout << "Rcv Len: " << rx.data() << " Size: " << rx.size() << std::endl;;
         return true;
    });
}
