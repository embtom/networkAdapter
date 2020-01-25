#include <gtest/gtest.h>
#include <iostream>
#include <cstring>
#include <tuple>
#include <algorithm>
#include <endian.h>
#include <EndianConvert.h>
#include <stream/StreamDataLink.hpp>

#define GTEST_BOX                   "[     cout ] "





using namespace EtNet;

TEST(DataLink, MoveKonstruktor)
{
    auto [a] = std::tuple(CStreamDataLink(3));
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}