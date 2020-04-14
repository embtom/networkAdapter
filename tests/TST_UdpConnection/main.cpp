#include <gtest/gtest.h>
#include <iostream>
#include <cstring>
#include <tuple>
#include <thread>
#include <Udp/UdpClient.hpp>
#include <BaseSocket.hpp>
#include <Udp/UdpServer.hpp>


#define ANSI_TXT_GRN                "\033[0;32m"
#define ANSI_TXT_MGT                "\033[0;35m" //Magenta
#define ANSI_TXT_DFT                "\033[0;0m" //Console default
#define GTEST_BOX                   "[     cout ] "
#define COUT_GTEST                  ANSI_TXT_GRN << GTEST_BOX
#define COUT_GTEST_MGT              COUT_GTEST << ANSI_TXT_DFT


using namespace EtNet;

struct STestData
{
    STestData() noexcept = default;
    explicit STestData(std::string _name, uint32_t _data0, uint16_t _data1, uint8_t _data2) noexcept :
        data0(_data0), data1(_data1), data2(_data2)  {
        std::memcpy(name, _name.c_str(), utils::array_count_v<decltype(name)>);
    }
    bool operator==(const STestData& rhs) const
    {
        for (int i = 0; i < utils::array_count_v<decltype(name)>; i++) 
        {
            if(rhs.name[i] != name[i]) {
                return false;
            }
        }
        if ((data0 == rhs.data0) &&
            (data1 == rhs.data1) &&
            (data2 == rhs.data2)) {
            return true;
        }
        return false;
    }
    char     name[10] {0};
    uint32_t data0 {0};
    uint16_t data1 {0};
    uint8_t  data2 {0};
};

class CDgramComTest : public  ::testing::Test
{
protected:
    CDgramComTest() :
        m_Server(std::move(CBaseSocket::SoReuseSocket(CBaseSocket(EtNet::ESocketMode::INET_DGRAM))),50002),
        m_Client(std::move(CBaseSocket::SoReuseSocket(CBaseSocket(EtNet::ESocketMode::INET_DGRAM))))
    { }

    CUdpServer m_Server;
    CUdpClient m_Client;
};

TEST_F(CDgramComTest, RawData)
{
    std::thread t([this]()
    {
        uint8_t rcvData[40] = {0};

        CUdpDataLink a = m_Server.waitForConnection();
        utils::span<uint8_t> rxtxData(rcvData);

        a.reciveFrom(rxtxData, [&a, &rcvData](EtNet::SPeerAddr ClientAddr, utils::span<uint8_t> rx)
        {
            std::cout << GTEST_BOX << "Rcv from: " << ClientAddr.Ip.toString() << " " << rcvData << std::endl;
            a.sendTo(ClientAddr, rx);
            return true;
        });
    });

    auto a = m_Client.getLink(std::string("localhost"),50002);
    std::string dataToSend ("hallo litte dgram test");
    uint8_t rcvData[40];
    utils::span<uint8_t> rxSpan (rcvData);
    a.send(utils::span(dataToSend).as_byte());
    a.reciveFrom(rxSpan, [] (EtNet::SPeerAddr ClientAddr, utils::span<uint8_t> rx)
    {
        return true;
    });
    std::string dataRcv((char*)rcvData);
    std::cout << GTEST_BOX << "Rcv: " << dataRcv << std::endl;

    EXPECT_EQ(dataRcv, dataToSend);

    t.join();
}

TEST_F(CDgramComTest, ComplexData)
{
    std::thread t([this]()
    {
        uint8_t rcvData[sizeof(STestData)] = {0};

        CUdpDataLink a = m_Server.waitForConnection();
        a.reciveFrom(utils::span<uint8_t>(rcvData), [&a, &rcvData](EtNet::SPeerAddr ClientAddr, utils::span<uint8_t> rx)
        {
            std::cout << GTEST_BOX << "Rcv from: " << ClientAddr.Ip.toString() << " " << rcvData << std::endl;
            a.sendTo(ClientAddr, rx);
            return true;
        });
    });

    STestData dataTransmit ("hallo", 0xFFBBCCDD, 0xAAEE, 0x88);
    STestData dataRecive;
    auto a = m_Client.getLink(std::string("localhost"),50002);

    a.send(utils::span(dataTransmit));
    a.reciveFrom(utils::span(dataRecive));

    EXPECT_EQ(dataTransmit, dataRecive);
    t.join();
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}