
#include <iostream>
#include <dgram/DgramServer.hpp>
#include <thread>


#define PORT_NUM 50002    

int main()
{
    auto baseSocket = EtNet::CBaseSocket(EtNet::ESocketMode::INET_DGRAM);
    EtNet::CDgramServer DgramServer(std::move(baseSocket), PORT_NUM);

    while (true)
    {
        uint8_t buffer [128];
        int rcvLen;
        EtNet::SClientAddr addr;

        auto [a] = DgramServer.waitForConnection();
        rcvLen = a.reciveFrom(buffer, sizeof(buffer), [&addr, &buffer, &DgramServer](EtNet::SClientAddr ClientAddr, std::size_t len) 
        {
            addr = std::move(ClientAddr);
            std::cout << "Message form: " << addr.Ip.toString() << " with length: " << len << std::endl;
           
            for (int j = 0; j < len; j++) {
                buffer[j] = toupper((unsigned char) buffer[j]);
            }
            return true;
        });
        a.sendTo(addr,(char*)buffer,rcvLen);
    }
}

