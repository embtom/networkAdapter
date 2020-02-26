
#include <iostream>
#include <BaseSocket.hpp>
#include <Stream/StreamServer.hpp>
#include <Stream/StreamDataLink.hpp>
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

        EtNet::CStreamDataLink a;
        EtNet::CIpAddress b;
        std::tie(a,b) = CStreamServer.waitForConnection();

        a.recive(rxSpan, [&a](utils::span<char> rx) {
            std::cout << "Rcv Len:" << rx.size() << std::endl;;

            for (char& elem : rx) {
                elem = toupper(elem);
            }

            a.send(rx);
            return true;
        });
    }

}
