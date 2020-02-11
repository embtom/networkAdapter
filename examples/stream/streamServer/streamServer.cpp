
#include <iostream>
#include <BaseSocket.hpp>
#include <stream/StreamServer.hpp>
#include <stream/StreamDataLink.hpp>
#undef min
#undef max


//#define INADDR_ANY ((unsigned long int) 0x00000000)

int main(int argc, char *argv[])
{
    auto baseSocket = EtNet::CBaseSocket(EtNet::ESocketMode::INET_STREAM);
    EtNet::CStreamServer CStreamServer(std::move(baseSocket), 5001);
    while(true)
    {
        uint8_t buffer [128];
        int rcvLen;
        auto [a,b] = CStreamServer.waitForConnection();

        rcvLen= a.recive(buffer, sizeof(buffer), [](std::size_t len) {
            std::cout << "Rcv Len:" << len << std::endl;;
            return true;
        });

        for (int j = 0; j < rcvLen; j++) {
            buffer[j] = toupper((unsigned char) buffer[j]);
        }
        a.send((char*)buffer,(std::size_t)rcvLen);
    }

}
