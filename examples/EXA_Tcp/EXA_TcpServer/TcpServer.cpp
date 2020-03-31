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
#include <BaseSocket.hpp>
#include <Tcp/TcpServer.hpp>
#include <Tcp/TcpDataLink.hpp>

//*****************************************************************************
//! \brief EXA_TcpServer
//!

int main(int argc, char *argv[])
{
    auto baseSocket = EtNet::CBaseSocket(EtNet::ESocketMode::INET_STREAM);
    EtNet::CTcpServer CTcpServer(std::move(baseSocket), 5001);

    EtNet::CTcpDataLink a;
    EtNet::CIpAddress b;
    std::tie(a,b) = CTcpServer.waitForConnection();

    while(true)
    {
        char buffer [128];
        utils::span<char> rxSpan(buffer);
        a.recive(rxSpan, [&a](utils::span<char> rx) {
            std::cout << "Rcv Len:" << rx.size() << std::endl;;

            for (char& elem : rx) {
                elem = toupper(elem);
            }

            a.send(rx);
            return true;
        });
    }

}
