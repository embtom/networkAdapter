
#include <iostream>
#include <stream/StreamClient.hpp>
#include <stream/StreamDataLink.hpp>
#include <HostName.h>
#include <algorithm>

int main(int argc, char *argv[])
{
    uint8_t buffer [128];

    auto baseSocket = EtNet::CBaseSocket(EtNet::ESocketMode::INET_STREAM);
    EtNet::CStreamClient streamClient(std::move(baseSocket));
//    EtNet::CStreamDataLink dataSocket = streamClient.connect("192.168.1.22",5001);
//    EtNet::CStreamDataLink dataSocket = streamClient.connect("tom-fujitsu",5001);
    auto [a] = streamClient.connect("localhost",5001);

    std::string test ("Hallo");
    a.send(test.c_str(),test.length());
    int rcvLen= a.recive(buffer, sizeof(buffer), [](std::size_t len) {
         std::cout << "Rcv Len:" << len << std::endl;;
         return true;
    });
    std::cout << "Rcv: " << (char*)buffer << std::endl;
}
