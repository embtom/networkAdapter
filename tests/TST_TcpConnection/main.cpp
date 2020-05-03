#include <gtest/gtest.h>
#include <iostream>
#include <cstring>
#include <tuple>
#include <thread>
#include <templateHelpers.h>
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

template <>
inline auto EtEndian::registerMembers<STestData>()
{
   return members(
      member("name",   &STestData::name),
      member("data0",  &STestData::data0),
      member("data1",  &STestData::data1),
      member("data2",   &STestData::data2)
   );
}


class CTcpComTest : public  ::testing::Test
{
protected:
    CTcpComTest() :
        m_Server(std::move(CBaseSocket::SoReuseSocket(CBaseSocket(EtNet::ESocketMode::INET_STREAM))),50003),
        m_Client(std::move(CBaseSocket::SoReuseSocket(CBaseSocket(EtNet::ESocketMode::INET_STREAM))))
    { }

    CTcpServer m_Server;
    CTcpClient m_Client;
};

TEST_F(CTcpComTest, RawData)
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
    a.send(utils::span(dataToSend));
    a.recive(rxSpan);
    std::string dataRcv((char*)rcvData);
    std::cout << GTEST_BOX << "Rcv: " << dataRcv << std::endl;

    EXPECT_EQ(dataRcv, dataToSend);

    t.join();
}

TEST_F(CTcpComTest, ComplexData)
{

    std::thread t([this]()
    {
        uint8_t rcvData[sizeof(STestData)] = {0};
        utils::span<uint8_t> rcvSpan (rcvData);

        CTcpDataLink a;
        CIpAddress b;

        std::tie(a, b) = m_Server.waitForConnection();
        std::cout << GTEST_BOX << "Server connectd from: " << b.toString() << std::endl;
        a.recive(rcvSpan, [&a, &rcvData](utils::span<uint8_t> rx)
        {
            std::cout << GTEST_BOX << "Rcv from: " << rx.size() << std::endl;
            a.send(rx);
            return true;
        });
    });

    STestData dataTransmit ("hallo", 0xFFBBCCDD, 0xAAEE, 0x88);
    STestData dataRecive;
    auto a = m_Client.connect(std::string("localhost"),50003);

    a.send(utils::span(dataTransmit));
    a.recive(utils::span(dataRecive));

    EXPECT_EQ(dataTransmit, dataRecive);
    t.join();
}

TEST_F(CTcpComTest, ComplexDataEndian)
{

    std::thread t([this]()
    {
        uint8_t rcvData[sizeof(STestData)] = {0};
        utils::span<uint8_t> rcvSpan (rcvData);

        CTcpDataLink a;
        CIpAddress b;

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

    STestData dataTransmit ("hallo", 0xFFBBCCDD, 0xAAEE, 0x88);
    a.send(EtEndian::CNetOrder(dataTransmit));

    EtEndian::CHostOrder<STestData> rx;
    a.recive(rx);
    const STestData& dataRecive = rx.HostOrder();

    EXPECT_EQ(dataTransmit, dataRecive);
    t.join();
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}