#include <gtest/gtest.h>
#include <iostream>
#include <BaseSocket.hpp>
#include <helper/SocketImpl.hpp>
#include <IpAddress.hpp>
#include <cstring>


TEST(CBaseSocket,INET_DGRAM )
{
    CSocketImpl testSocket;
    try {
        testSocket = std::move(CSocketImpl(ESocketMode::INET_DGRAM));
        
        //CBaseSocket testSocket2 = std::move(CBaseSocket::ReuseSocket(CSocketImpl(ESocketMode::INET_DGRAM)));

    }
    catch(const std::exception& e){
        FAIL() << e.what() << '\n';
    }

    int fd = testSocket.getFd();
    EXPECT_GT(fd, 0);

    ESocketMode reqMode = testSocket.testSocketMode();
    EXPECT_EQ(reqMode,ESocketMode::INET_DGRAM);
}

TEST(CBaseSocket,INET_STREAM )
{
    CSocketImpl testSocket;
    try {
        testSocket = std::move(CSocketImpl(ESocketMode::INET_STREAM));
    }
    catch(const std::exception& e){
        FAIL() << e.what() << '\n';
    }

    int fd = testSocket.getFd();
    EXPECT_GT(fd, 0);

    ESocketMode reqMode = testSocket.testSocketMode();
    EXPECT_EQ(reqMode,ESocketMode::INET_STREAM);
}

TEST(CBaseSocket,INET6_DGRAM )
{
    CSocketImpl testSocket;
    try {
        testSocket = std::move(CSocketImpl(ESocketMode::INET6_DGRAM));
    }
    catch(const std::exception& e){
        FAIL() << e.what() << '\n';
    }

    int fd = testSocket.getFd();
    EXPECT_GT(fd, 0);

    ESocketMode reqMode = testSocket.testSocketMode();
    EXPECT_EQ(reqMode,ESocketMode::INET6_DGRAM);
}

TEST(CBaseSocket,INET6_STREAM )
{
    CSocketImpl testSocket;
    try {
        testSocket = std::move(CSocketImpl(ESocketMode::INET6_STREAM));
    }
    catch(const std::exception& e){
        FAIL() << e.what() << '\n';
    }

    int fd = testSocket.getFd();
    EXPECT_GT(fd, 0);

    ESocketMode reqMode = testSocket.testSocketMode();
    EXPECT_EQ(reqMode,ESocketMode::INET6_STREAM);
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}