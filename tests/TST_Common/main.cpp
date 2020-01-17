#include <gtest/gtest.h>
#include <iostream>
#include <cstring>
#include <tuple>
#include <algorithm>
#include <endian.h>
#include <EndianConvert.h>
#include <stream/StreamDataLink.hpp>

#define GTEST_BOX                   "[     cout ] "

template <typename T, std::enable_if_t<std::is_arithmetic<T>::value,int> = 0>
T swapEndian(T value)
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    union {
        T val;
        std::array<uint8_t ,sizeof(T)> raw;
    } src, dst;

    src.val = value;
    std::reverse_copy(src.raw.begin(), src.raw.end(), dst.raw.begin());
    return dst.val;
#else
    return value;
#endif
}



using namespace EtNet;

TEST(DataLink, MoveKonstruktor)
{
    auto [a] = std::tuple(CStreamDataLink(3));
}

TEST(Endian, toNetworkByteOrder)
{
    {
        std::cout << std::hex;

        uint16_t a = 5;
        uint16_t b = EtEndian::host_to_network(a);
        uint16_t verify = swapEndian(a);
        std::cout << GTEST_BOX << "A: " << a << " B: " << b << std::endl;

        uint16_t c = EtEndian::network_to_host(b);
        std::cout << GTEST_BOX << "B: " << b << " C: " << c << std::endl;
        EXPECT_EQ(b,verify);
        EXPECT_EQ(a,c);
    }
    {
        std::cout << std::hex;

        uint32_t a = 0x01020304;
        uint32_t b = EtEndian::host_to_network(a);
        std::cout << GTEST_BOX << "A: " << a << " B: " << b << std::endl;
        uint32_t verify = swapEndian(a);

        uint32_t c = EtEndian::network_to_host(b);
        std::cout << GTEST_BOX << "B: " << b << " C: " << c << std::endl;
        EXPECT_EQ(b,verify);
        EXPECT_EQ(a,c);
    }
    {
        std::cout << std::hex;

        uint64_t a = 0x01020304;
        uint64_t b = EtEndian::host_to_network(a);
        std::cout << GTEST_BOX << "A: " << a << " B: " << b << std::endl;
        uint64_t verify = swapEndian(a);

        uint32_t c = EtEndian::network_to_host(b);
        std::cout << GTEST_BOX << "B: " << b << " C: " << c << std::endl;
        EXPECT_EQ(b,verify);
        EXPECT_EQ(a,c);
    }

}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}