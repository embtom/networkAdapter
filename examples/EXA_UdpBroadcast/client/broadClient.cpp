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
#include <thread>
#include <threadLoop.h>
#include <BaseSocket.hpp>
#include <Lookup/InterfacesLookup.hpp>
#include <Udp/UdpClient.hpp>


#define PORT_NUM 50002

using namespace EtNet;

int main(int argc, char *argv[])
{
    char buffer [128];
    utils::span<char> rxSpan(buffer);

    CIpAddress::IpAddresses broadIp = CNetInterface::getAllIpv4Broadcast();

    std::cout << "Broadcast Address to send is: " << broadIp.at(0).toString() << std::endl;

    auto baseSocket = CBaseSocket::SoBroadcast(
                      CBaseSocket::SoReuseSocket(
                      CBaseSocket(ESocketMode::INET_DGRAM)));
    EtNet::CUdpClient UdpClient(std::move(baseSocket));

    unsigned rcvCount {0};
    CUdpDataLink a = UdpClient.getLink(broadIp.at(0).toString(), PORT_NUM);
    auto rcvFunc = [&a, &rcvCount]()
    {
        char buffer [128];
        utils::span<char> rxSpan(buffer);
        CUdpDataLink::ERet ret = a.reciveFrom(rxSpan, [&rcvCount](SPeerAddr peer, utils::span<char> rx) {
            std::cout << "RCV from " << peer.Ip.toString() << std::endl;
            std::cout << "Rcv Len: " << rx.data() << " Size: " << rx.size() << std::endl;
            rcvCount++;
            return true;
        });

        if (CUdpDataLink::ERet::UNBLOCK == ret) {
            std::cout << "finish" << std::endl;
            return true;
        }
        std::cout << "Called " << rcvCount << std::endl;
        return false;
    };
    utils::CThreadLoop rcvLoop (rcvFunc, "RX_Worker");
    rcvLoop.start(std::chrono::milliseconds::zero());

    for (int i = 0 ; i < 2; i++) {
        std::string test = std::string("Hallo") + std::to_string(i);
        a.send(utils::span<char>(test));
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));
    a.unblockRecive();
    rcvLoop.waitUntilFinished();
}
