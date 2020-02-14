
#include <iostream>
#include <BaseSocket.hpp>
#include <stream/StreamServer.hpp>
#include <stream/StreamDataLink.hpp>
#include <span.h>
#undef min
#undef max


//#define INADDR_ANY ((unsigned long int) 0x00000000)

int main(int argc, char *argv[])
{
    auto baseSocket = EtNet::CBaseSocket(EtNet::ESocketMode::INET_STREAM);
    EtNet::CStreamServer CStreamServer(std::move(baseSocket), 5001);
    while(true)
    {
        char buffer [128];
        utils::span<char> rxSpan(buffer);
        int rcvLen;
        auto [a,b] = CStreamServer.waitForConnection();

        a.recive(rxSpan, [&buffer](utils::span<char> rx) {
            std::cout << "Rcv Len:" << rx.size() << std::endl;;


            // for (int j = 0; j < rx.size(); j++) {
            //     buffer[j] = toupper((unsigned char) buffer[j]);
            // }

            return true;
        });

  


        a.send(utils::span<char>((char*)buffer,rcvLen));
    }

}
