/*
 * This file is part of the EMBTOM project
 * Copyright (c) 2018-2020 Thomas Willetal
 * (https://github.com/embtom)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * DataLink copy of this software and associated documentation files (the
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
#include <span.h>
#include <threadLoop.h>
#include <BaseSocket.hpp>
#include <Udp/UdpClient.hpp>
#include <Lookup/InterfacesLookup.hpp>
#include <ComProto.hpp>

constexpr int PORT_NUM = 50002;

using namespace EtNet;
using namespace EtEndian;

//*****************************************************************************
//! \brief EXA_Broadclient
//!

int main(int argc, char *argv[])
{
    ////////////////////////////////
    // Send Broadcast Message to Server
    CIpAddress::IpAddresses broadIp = CNetInterface::getAllIpv4Broadcast();
    std::cout << "Broadcast Address to send is: " << broadIp.at(0).toString() << std::endl;

    auto baseSocket = CBaseSocket::SoBroadcast(
                      CBaseSocket::SoReuseSocket(
                      CBaseSocket(ESocketMode::INET_DGRAM)));
    CUdpClient UdpClient(std::move(baseSocket));

    CUdpDataLink DataLink;
    try {
        DataLink = UdpClient.getLink(broadIp.at(0).toString(), PORT_NUM);
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

    std::this_thread::sleep_for(std::chrono::seconds(3));
    DataLink.unblockRecive();
    rcvLoop.waitUntilFinished();

    return EXIT_SUCCESS;
}
