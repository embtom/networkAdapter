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
#include <array>
#include <span.h>
#include <BaseSocket.hpp>
#include <Udp/UdpServer.hpp>
#include <Udp/UdpDataLink.hpp>
#include <ComProto.hpp>

constexpr int PORT_NUM = 50002;

using namespace EtNet;

//*****************************************************************************
//! \brief EXA_UdpServer
//!
int main()
{
    auto baseSocket = CBaseSocket(ESocketMode::INET_DGRAM);
    CUdpServer UdpServer(std::move(baseSocket), PORT_NUM);

    while (true)
    {
        CUdpDataLink a = UdpServer.waitForConnection();
        SPeerAddr activePeerAddr;

        EtEndian::CHostOrder<ComProto> rx;
        a.reciveFrom(rx, [&activePeerAddr] (SPeerAddr ClientAddr, utils::span<uint8_t> rx)
        {
            activePeerAddr = ClientAddr;
            std::cout << "Connected from: " << ClientAddr.Ip.toString() << std::endl;
            return false;
        });

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

        ComProto txData;
        int i;
        for(i = 0; i < strlen(rxData.info); i++) {
            txData.info[i] = toupper(rxData.info[i]);
        }
        memcpy(&txData.info[i],"Echo\0",5);
        txData.data1 = rxData.data1;
        txData.data2 = rxData.data2;
        txData.disconnect = rxData.disconnect;
        if(rxData.disconnect) {
            std::cout << "Last Message from peer" << std::endl;
        }

        a.sendTo(activePeerAddr, EtEndian::CNetOrder(txData));
    }
}
