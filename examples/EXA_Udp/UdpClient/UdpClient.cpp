/*
 * This file is part of the EMBTOM project
 * Copyright (c) 2018-2020 Thomas Willetal
 * (https://github.com/embtom)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//******************************************************************************
// Headers

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <array>
#include <string>
#include <map>
#include <NetOrder.h>
#include <HostOrder.h>
#include <docopt.h>
#include <span.h>
#include <threadLoop.h>
#include <BaseSocket.hpp>
#include <Udp/UdpClient.hpp>
#include <ComProto.hpp>

constexpr int PORT_NUM = 50002;

using namespace EtNet;
using namespace EtEndian;

//*****************************************************************************
//! \brief EXA_UdpClient
//!

int main(int argc, char *argv[])
{
    constexpr std::string_view docOptCmd =
    R"(EXA_UdpClient.
        Usage:
        EXA_UdpClient (-c | --connect) <hostname>
        EXA_UdpClient (-h | --help)
        EXA_UdpClient --version
        Options:
        -h --help     Show this screen.
        --version     Show version.
    )";

    constexpr auto networkAdapterVersion = "networkAdapter " NETWORKING_ADAPTER_VERSION;
    using ArgMap_t = std::map<std::string, docopt::value>;
    ArgMap_t args = docopt::docopt(std::string(docOptCmd),
                                   { argv + 1, argv + argc },
                                   true,
                                   networkAdapterVersion);
    if (!(args["<hostname>"])) {
        return 0;
    }

    ////////////////////////////////
    // Connect Server
    auto baseSocket = CBaseSocket(CBaseSocket::SoReuseSocket(ESocketMode::INET_DGRAM));
    CUdpClient UdpClient (std::move(baseSocket));

    std::string hostName(args["<hostname>"].asString());
    std::cout << "try to connect to: " << hostName << std::endl;

    CUdpDataLink DataLink;
    try {
        DataLink = UdpClient.getLink(hostName, PORT_NUM);
    }
    catch(const std::exception& e) {
        std::cout << "Failed to establish connection: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    ////////////////////////////////
    // Rcv Loop
    unsigned rcvCount {0};
    auto rcvFunc = [&DataLink, &rcvCount]()
    {
        EtEndian::CHostOrder<ComProto> rx;
        if(CUdpDataLink::ERet::UNBLOCK == DataLink.reciveFrom(rx))
        {
            std::cout << "finish" << std::endl;
            return true;
        }

        const ComProto& rxData = rx.HostOrder();

        EtEndian::doForAllMembers<ComProto>(
            [&rxData](const auto& member)
        {
            using MemberT = EtEndian::get_member_type<decltype(member)>;
            std::cout << std::hex << " " << std::left
                      << std::setw(20) << member.getName()
                      << std::setw(20) << member.getConstRef(rxData)
                      << std::endl;
        });
        std::cout << "----" << std::endl;
        return false;
    };

    utils::CThreadLoop rcvLoop (rcvFunc, "RX_Worker");
    rcvLoop.start(std::chrono::milliseconds::zero());

    ////////////////////////////////
    // Tx
    ComProto tx {"Hallo", 0 ,0, 0};
    for (int i = 0; i < 10; i++)
    {
        tx.data1 = 0xffaa0000 + i;
        tx.data2 = 0xAA00 + i;
        DataLink.send(CNetOrder(tx));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    tx.disconnect = 1;

    std::this_thread::sleep_for(std::chrono::seconds(5));
    DataLink.unblockRecive();
    rcvLoop.waitUntilFinished();

    return EXIT_SUCCESS;
}