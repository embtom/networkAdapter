/*
 * This file is part of the EMBTOM project
 * Copyright (c) 2018-2019 Thomas Willetal
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
#include <docopt.h>
#include <BaseSocket.hpp>
#include <Udp/UdpClient.hpp>

#define PORT_NUM 50002
#define BUF_SIZE 100

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

    uint8_t buffer [128];
    utils::span<uint8_t> rxSpan(buffer);

    /* Create a datagram socket; send to an address in the IPv6 domain */
    auto baseSocket = EtNet::CBaseSocket(EtNet::CBaseSocket::SoReuseSocket(EtNet::ESocketMode::INET_DGRAM));
    EtNet::CUdpClient abc (std::move(baseSocket));

    std::string hostName(args["<hostname>"].asString());
    std::cout << "try to connect to: " << hostName << std::endl;

    EtNet::CUdpDataLink d;
    try {
        d = abc.getLink(hostName, PORT_NUM);
    }
    catch(const std::exception& e) {
        std::cout << "Failed to establish connection: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    /* Send messages to server; echo responses on stdout */
    for (int j = 0; j < 2; j++) {

        std::string test = std::string("Hallo") + std::to_string(j);
        d.send(utils::span(test).as_byte());

        d.reciveFrom(rxSpan, [](EtNet::SPeerAddr ClientAddr, utils::span<uint8_t> rx) {
            std::cout << "Rcv Len: " << rx.data() << " Size: " << rx.size() << std::endl;
            return true;
        });
    }

    return EXIT_SUCCESS;
}