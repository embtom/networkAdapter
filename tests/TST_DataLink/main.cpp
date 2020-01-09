#include <gtest/gtest.h>
#include <iostream>
#include <stream/StreamDataLink.hpp>
#include <cstring>
#include <tuple>

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