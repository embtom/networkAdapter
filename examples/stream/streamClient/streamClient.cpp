
#include <iostream>
#include <stream/StreamClient.hpp>
#include <stream/StreamDataLink.hpp>
#include <algorithm>
#include <span.h>

int main(int argc, char *argv[])
{
    char buffer [128];
    utils::span<char> rxSpan(buffer);

    auto baseSocket = EtNet::CBaseSocket(EtNet::ESocketMode::INET_STREAM);
    EtNet::CStreamClient streamClient(std::move(baseSocket));
//    EtNet::CStreamDataLink dataSocket = streamClient.connect("192.168.1.22",5001);
//    EtNet::CStreamDataLink dataSocket = streamClient.connect("tom-fujitsu",5001);
    auto [a] = streamClient.connect("localhost",5001);

    std::string test ("Hallo");
    a.send(utils::span<char>(test));
    a.recive(rxSpan, [] (utils::span<char> rx) {
         std::cout << "Rcv Len: " << rx.data() << " Size: " << rx.size() << std::endl;;
         return true;
    });
}
