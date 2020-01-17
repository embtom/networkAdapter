#include <gtest/gtest.h>
#include <iostream>
#include <cstring>
#include <tuple>
#include <thread>
#include <BaseSocket.hpp>
#include <stream/StreamClient.hpp>
#include <stream/StreamServer.hpp>


#define ANSI_TXT_GRN                "\033[0;32m"
#define ANSI_TXT_MGT                "\033[0;35m" //Magenta
#define ANSI_TXT_DFT                "\033[0;0m" //Console default
#define GTEST_BOX                   "[     cout ] "
#define COUT_GTEST                  ANSI_TXT_GRN << GTEST_BOX  
#define COUT_GTEST_MGT              COUT_GTEST << ANSI_TXT_DFT


using namespace EtNet;

class CStreamComTest : public  ::testing::Test 
{
protected:
    CStreamComTest() :
        m_Server(std::move(CBaseSocket::SoReuseSocket(CBaseSocket(EtNet::ESocketMode::INET_STREAM))),50003),
        m_Client(std::move(CBaseSocket::SoReuseSocket(CBaseSocket(EtNet::ESocketMode::INET_STREAM))))
    { }

    CStreamServer m_Server;
    CStreamClient m_Client;
};

TEST_F(CStreamComTest, simple)
{
    std::thread t([this]()
    {   
        char rcvData[40];
        CStreamDataLink a; 
        CIpAddress b;
        //auto [a, b] = m_Server.waitForConnection();
        std::tie(a, b) = m_Server.waitForConnection();
        std::cout << GTEST_BOX << "Server connectd from: " << b.toString() << std::endl;
        a.recive(reinterpret_cast<uint8_t*>(&rcvData[0]), sizeof(rcvData), [&a, &rcvData](std::size_t len)
        {
            std::cout << GTEST_BOX << "Rcv from: " << rcvData << std::endl;
            a.send(&rcvData[0], strlen(rcvData));
            return true;
        });
    });

    auto [a] = m_Client.connect(std::string("localhost"),50003);
    std::string dataToSend ("hallo litte stream test");
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