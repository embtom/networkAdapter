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
#include <array>
#include <span.h>
#include <BaseSocket.hpp>
#include <Tcp/TcpServer.hpp>
#include <Tcp/TcpDataLink.hpp>
#include <ComProto.hpp>

constexpr int PORT_NUM = 5001;

using namespace EtNet;

//*****************************************************************************
//! \brief EXA_TcpServer
//!
int main(int argc, char *argv[])
{
    auto baseSocket = CBaseSocket(ESocketMode::INET_STREAM);
    CTcpServer CTcpServer(std::move(baseSocket), PORT_NUM);

    CTcpDataLink DataLink;
    CIpAddress peerIp;

    while (true)
    {
        std::cout<< "Wait for connection" << std::endl;
        std::tie(DataLink,peerIp) = CTcpServer.waitForConnection();
        std::cout << "Connected by: " << peerIp.toString() << std::endl;

        bool serverActive = true;
        while(serverActive)
        {
            EtEndian::CHostOrder<ComProto> rx;
            DataLink.recive(rx);
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
            memcpy(&txData.info[i],"Echo",4);
            txData.data1 = rxData.data1;
            txData.data2 = rxData.data2;
            txData.disconnect = rxData.disconnect;
            if(rxData.disconnect) {
                std::cout << "Disconntected" << std::endl;
                serverActive = false;
            }
            DataLink.send(EtEndian::CNetOrder(txData));
        }
    }
}
