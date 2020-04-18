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
#include <algorithm>
#include <array>
#include <string>
#include <map>
#include <NetOrder.h>
#include <docopt.h>
#include <span.h>
#include <threadLoop.h>
#include <BaseSocket.hpp>
#include <Tcp/TcpClient.hpp>
#include <Tcp/TcpDataLink.hpp>

using namespace EtNet;

//*****************************************************************************
//! \brief EXA_TcpClient
//!

struct dataTx
{
    std::string info;
    uint32_t data1;
    uint16_t data2;
};


template <>
inline auto EtEndian::registerMembers<dataTx>()
{
   return members(
      member("info",   &dataTx::info),
      member("data1",   &dataTx::data1),
      member("data2",   &dataTx::data2)

   );
}

using namespace EtEndian;


int main(int argc, char *argv[])
{
    constexpr std::string_view docOptCmd =
        R"(EXA_TcpClient.
            Usage:
            EXA_TcpClient (-c | --connect) <hostname>
            EXA_TcpClient (-h | --help)
            EXA_TcpClient --version
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

    auto baseSocket = CBaseSocket(ESocketMode::INET_STREAM);
    CTcpClient TcpClient(std::move(baseSocket));

    std::string hostName(args["<hostname>"].asString());
    std::cout << "try to connect to: " << hostName << std::endl;

    EtNet::CTcpDataLink a;
    try {
        a = TcpClient.connect(hostName, 5001);
    }
    catch(const std::exception& e) {
        std::cout << "Failed to establish connection: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    unsigned rcvCount {0};
    auto rcvFunc = [&a, &rcvCount]()
    {
        uint8_t buffer [128];
        utils::span<uint8_t> rxSpan(buffer);
        CTcpDataLink::ERet ret = a.recive(rxSpan, [&rcvCount] (utils::span<uint8_t> rx) {
            std::cout << "Rcv Len: " << rx.data() << " Size: " << rx.size() << std::endl;
            rcvCount++;
            return true;
        });

        if (CTcpDataLink::ERet::UNBLOCK == ret) {
            std::cout << "finish" << std::endl;
            return true;
        }

        std::cout << "Called " << rcvCount << std::endl;
        return false;
    };

    utils::CThreadLoop rcvLoop (rcvFunc, "RX_Worker");
    rcvLoop.start(std::chrono::milliseconds::zero());


    dataTx tx {"Hallo", 0 ,0};

    std::string toSend = std::string("Hallo");
    for (int i = 0; i < 10; i++)
    {
        tx.data1 = 0xffaa0000 + i;
        tx.data2 = 0xAA00 + i;
        dataTx netOrderTx = CNetOrder(tx).NetworkOrder();

        utils::span<dataTx> bla (dataTx);

       // a.send(utils::span<char>(netOrderTx));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
    a.unblockRecive();
    rcvLoop.waitUntilFinished();
}
