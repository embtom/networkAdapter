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
#include <array>
#include <span.h>
#include <Udp/UdpServer.hpp>
#include <thread>

#define PORT_NUM 50002

//*****************************************************************************
//! \brief EXA_UdpServer
//!

int main()
{
    auto baseSocket = EtNet::CBaseSocket(EtNet::ESocketMode::INET_DGRAM);
    EtNet::CUdpServer UdpServer(std::move(baseSocket), PORT_NUM);

    while (true)
    {
        uint8_t buffer [128] = {0};
        utils::span<uint8_t> rxtxSpan (buffer);

        EtNet::CUdpDataLink a = UdpServer.waitForConnection();
        a.reciveFrom(rxtxSpan,[&a, &UdpServer](EtNet::SPeerAddr ClientAddr, utils::span<uint8_t> rx)
        {
            std::cout << "Message form: " << ClientAddr.Ip.toString() << " with length: " << rx.size() << std::endl;

            for(auto &elem : rx) {
                elem = toupper(elem);
            }

            a.sendTo(ClientAddr, rx);
            return true;
        });

    }
}
