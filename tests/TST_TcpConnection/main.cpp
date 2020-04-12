#include <gtest/gtest.h>
#include <iostream>
#include <cstring>
#include <tuple>
#include <thread>
#include <BaseSocket.hpp>
#include <Tcp/TcpClient.hpp>
#include <Tcp/TcpServer.hpp>


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

    CTcpServer m_Server;
    CTcpClient m_Client;
};

TEST_F(CStreamComTest, simple)
{
    std::thread t([this]()
    {
        uint8_t rcvData[40] = {0};
        utils::span<uint8_t> rcvSpan (rcvData);

        CTcpDataLink a;
        CIpAddress b;
        //auto [a, b] = m_Server.waitForConnection();
        std::tie(a, b) = m_Server.waitForConnection();
        std::cout << GTEST_BOX << "Server connectd from: " << b.toString() << std::endl;
        a.recive(rcvSpan, [&a, &rcvData](utils::span<uint8_t> rx)
        {
            std::cout << GTEST_BOX << "Rcv from: " << rx.size() << std::endl;
            a.send(rx);
            return true;
        });
    });

    auto a = m_Client.connect(std::string("localhost"),50003);
    std::string dataToSend ("hallo litte stream test");
    uint8_t rcvData[40];
    utils::span<uint8_t> rxSpan (rcvData);
    a.send(utils::span(dataToSend).as_byte());
    a.recive(rxSpan);
    std::string dataRcv((char*)rcvData);
    std::cout << GTEST_BOX << "Rcv: " << dataRcv << std::endl;

    EXPECT_EQ(dataRcv, dataToSend);

    t.join();
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}