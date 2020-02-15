#include <gtest/gtest.h>
#include <iostream>
#include <HostName.h>
#include <IpAddress.hpp>
#include <cstring>

using namespace EtNet;

#define GTEST_BOX                   "[     cout ] "

TEST(CIpAddress, IPv4_in_addr)
{
    in_addr ipv4Addr;
    ipv4Addr.s_addr =   0x0100007F;

    CIpAddress ipv4Container(ipv4Addr);
    std::string strIpv4 = ipv4Container.toString();
    EXPECT_EQ(strIpv4, "127.0.0.1");
    EXPECT_EQ(ipv4Container.is_v4(), true);
    EXPECT_EQ(ipv4Container.is_v6(), false);

    const in_addr *ip4 = ipv4Container.to_v4();
    EXPECT_TRUE( 0 == std::memcmp(&ipv4Addr.s_addr, ip4, sizeof(in_addr)));

    const in6_addr *ip6 = ipv4Container.to_v6();
    EXPECT_EQ(ip6, nullptr);

    std::string strIp4 = ipv4Container.toString();
    EXPECT_EQ(strIp4, "127.0.0.1");
}

TEST(CIpAddress, IPv6_in_addr)
{
    in6_addr ipv6Addr;
    ipv6Addr.__in6_u.__u6_addr32[0] = 253;
    ipv6Addr.__in6_u.__u6_addr32[1] = 0;
    ipv6Addr.__in6_u.__u6_addr32[2] = 4280209102;
    ipv6Addr.__in6_u.__u6_addr32[3] = 3039800830;

    CIpAddress ipv6Container(ipv6Addr);
    std::string strIpv6 = ipv6Container.toString();
    EXPECT_EQ(strIpv6, "fd00::cece:1eff:fead:2fb5");
    EXPECT_EQ(ipv6Container.is_v4(), false);
    EXPECT_EQ(ipv6Container.is_v6(), true);

    const in_addr *ip4 = ipv6Container.to_v4();
    EXPECT_EQ(ip4, nullptr);

     const in6_addr *ip6 = ipv6Container.to_v6();
    EXPECT_TRUE( 0 == std::memcmp(&ipv6Addr, ip6, sizeof(in6_addr)));

     std::string strIp6 = ipv6Container.toString();
    EXPECT_EQ(strIp6, "fd00::cece:1eff:fead:2fb5");
}

TEST(CIpAddress, IPv4_string)
{
    std::string ipStr("192.168.0.1");
    CIpAddress toIpv4 (ipStr);
    std::string strIp4 = toIpv4.toString();
    EXPECT_EQ(strIp4, "192.168.0.1");
    EXPECT_EQ(toIpv4.is_v4(), true);
    EXPECT_EQ(toIpv4.is_v6(), false);
}

TEST(CIpAddress, IPv6_string)
{
    CIpAddress toIpv6(std::string("2003:f2:93cd:e100:9131:8000:5cf5:9f0c"));
    std::string strIp6 = toIpv6.toString();
    EXPECT_EQ(strIp6, "2003:f2:93cd:e100:9131:8000:5cf5:9f0c");
    EXPECT_EQ(toIpv6.is_v4(), false);
    EXPECT_EQ(toIpv6.is_v6(), true);
}

TEST(CIpAddress, IPv6_CopyConstruktor)
{
    CIpAddress Ipv6(std::string("2003:f2:93cd:e100:9131:8000:5cf5:9f0c"));
    CIpAddress Ipv6Copy(Ipv6);
    EXPECT_TRUE(0== std::memcmp(Ipv6.to_v6(), Ipv6Copy.to_v6(), sizeof(in6_addr)));

    CIpAddress MoveTest(std::move(Ipv6Copy));
    EXPECT_EQ(Ipv6Copy.is_v4(), false);
    EXPECT_EQ(Ipv6Copy.is_v6(), true);
    EXPECT_EQ(Ipv6Copy.to_v4(), nullptr);
    EXPECT_NE(Ipv6Copy.to_v6(), nullptr);
}

TEST(CIpAddress, IPv6_MoveConstruktor)
{
    CIpAddress Ipv6(std::string("2003:f2:93cd:e100:9131:8000:5cf5:9f0c"));
    CIpAddress Ipv6Move;

    EXPECT_EQ(Ipv6.is_v4(), false);
    EXPECT_EQ(Ipv6.is_v6(), true);
    EXPECT_EQ(Ipv6.to_v4(), nullptr);
    EXPECT_NE(Ipv6.to_v6(), nullptr);

    Ipv6Move = std::move(Ipv6);

    EXPECT_EQ(Ipv6.is_v4(), false);
    EXPECT_EQ(Ipv6.is_v6(), true);
    EXPECT_EQ(Ipv6.to_v4(), nullptr);
    EXPECT_NE(Ipv6.to_v6(), nullptr);
    EXPECT_EQ(Ipv6Move.is_v4(), false);
    EXPECT_EQ(Ipv6Move.is_v6(), true);
    EXPECT_EQ(Ipv6Move.to_v4(), nullptr);
    EXPECT_NE(Ipv6Move.to_v6(), nullptr);
}

TEST(CIpAddress, CompareOperator)
{
    in6_addr ipv6Addr;
    ipv6Addr.__in6_u.__u6_addr32[0] = 253;
    ipv6Addr.__in6_u.__u6_addr32[1] = 0;
    ipv6Addr.__in6_u.__u6_addr32[2] = 4280209102;
    ipv6Addr.__in6_u.__u6_addr32[3] = 3039800830;
    CIpAddress cmp1(ipv6Addr);
    CIpAddress cmp2(std::string("fd00::cece:1eff:fead:2fb5"));
    CIpAddress cmp3(std::string ("192.168.0.1"));
    CIpAddress cmp4(std::string("2003:f2:93cd:e100:9131:8000:5cf5:9f0c"));

    in_addr ipv4Addr;
    ipv4Addr.s_addr =   0x0100007F;
    CIpAddress cmp5(ipv4Addr);

    EXPECT_EQ(cmp1 == cmp2, true);
    EXPECT_EQ(cmp1 == cmp3, false);
    EXPECT_EQ(cmp1 == cmp4, false);
    EXPECT_EQ(cmp3 != cmp5, true);
}

TEST(CIpAddress, calculateBroadcast)
{
    CIpAddress ipv4 (std::string("192.168.0.1"));
    CIpAddress ipv4Submask (std::string("255.255.0.0"));
    CIpAddress ipv6 (std::string("2003:f2:93cd:e100:9131:8000:5cf5:9f0c"));
    CIpAddress broadcastIpv4 = ipv4.broadcast(ipv4Submask);
    EXPECT_EQ(broadcastIpv4,CIpAddress(std::string("192.168.255.255")));
}


TEST(HostName, NameToIP)
{
    CHostLookup host(std::string("localhost"));


    CHostLookup::IpAddresses ipList = host.addresses();

    auto it = std::find_if(ipList.begin(), ipList.end(),[] (const auto &elm)
    {
        if (elm.is_v4()) {
            std::cout << GTEST_BOX << elm.toString() << std::endl;
            return true;
        }
        else if (elm.is_v6()) {
            std::cout << GTEST_BOX << elm.toString() << std::endl;
            return true;
        }
        else {
            return false;
        }
    });

    EXPECT_NE(it, ipList.end());

    // std::string ipAddr = list.at(0).toString();
    // EXPECT_EQ(ipAddr, "127.0.0.1");

    // CHostLookup google(std::string("google.de"));
    // for(auto e : google.addresses())
    // {
    //     if(e.is_v4()) {
    //         std::cout << "Ip4 : " << e.toString() << std::endl;
    //     } else {
    //         std::cout << "Ip6 : " << e.toString() << std::endl;
    //     }

    // }

    // CHostLookup fritzBox(std::string("tom-fujitsu"));
    // for(auto e : fritzBox.addresses())
    // {
    //     if(e.is_v4()) {
    //         std::cout << "Ip4 : " << e.toString() << std::endl;
    //     } else {
    //         std::cout << "Ip6 : " << e.toString() << std::endl;
    //     }

    // }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}