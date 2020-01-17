#include <gtest/gtest.h>
#include <iostream>
#include <cstring>
#include <tuple>
#include <thread>
#include <dgram/DgramClient.hpp>
#include <BaseSocket.hpp>
#include <dgram/DgramServer.hpp>


#define ANSI_TXT_GRN                "\033[0;32m"
#define ANSI_TXT_MGT                "\033[0;35m" //Magenta
#define ANSI_TXT_DFT                "\033[0;0m" //Console default
#define GTEST_BOX                   "[     cout ] "
#define COUT_GTEST                  ANSI_TXT_GRN << GTEST_BOX  
#define COUT_GTEST_MGT              COUT_GTEST << ANSI_TXT_DFT


using namespace EtNet;

class CDgramComTest : public  ::testing::Test 
{
protected:
    CDgramComTest() :
        m_Server(std::move(CBaseSocket::SoReuseSocket(CBaseSocket(EtNet::ESocketMode::INET_DGRAM))),50002),
        m_Client(std::move(CBaseSocket::SoReuseSocket(CBaseSocket(EtNet::ESocketMode::INET_DGRAM))))
    { }

    CDgramServer m_Server;
    CDgramClient m_Client;
};

TEST_F(CDgramComTest, simple)
{
    std::thread t([this]()
    {   
        char rcvData[40];
        CDgramDataLink a;
        std::tie(a) = m_Server.waitForConnection();
        //auto (a) = m_Server.waitForConnection();

        a.reciveFrom(reinterpret_cast<uint8_t*>(&rcvData[0]), sizeof(rcvData), [&a, &rcvData](EtNet::SPeerAddr ClientAddr, std::size_t len) 
        {
            std::cout << GTEST_BOX << "Rcv from: " << ClientAddr.Ip.toString() << " " << rcvData << std::endl;
            a.sendTo(ClientAddr, &rcvData[0], strlen(rcvData));
            return true;
        });
    });

    auto [a, b] = m_Client.getLink(std::string("localhost"),50002);
    std::string dataToSend ("hallo litte dgram test");
    char rcvData[40];
    a.send(dataToSend.c_str(),dataToSend.length());
    a.recive(reinterpret_cast<uint8_t*>(&rcvData[0]),sizeof(rcvData));
    std::string dataRcv(rcvData); 
    std::cout << GTEST_BOX << "Rcv: " << dataRcv << std::endl;

    EXPECT_EQ(dataRcv, dataToSend);

    t.join();
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}