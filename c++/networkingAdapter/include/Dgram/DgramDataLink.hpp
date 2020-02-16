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
 * THE SOFTWARE IS PROVIDED "AS IS", WITdgramClientHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _DGRAMDATALINK_H_
#define _DGRAMDATALINK_H_

//******************************************************************************
// Header

#include <stdint.h>
#include <cstddef>
#include <functional>
#include <span.h>

#include <BaseSocket.hpp>
#include <IpAddress.hpp>
#include <BaseDataLink.hpp>

namespace EtNet
{

struct SPeerAddr
{
    CIpAddress Ip;
    unsigned int Port {0};
};

constexpr auto defaultReciveFrom = [](SPeerAddr ClientAddr, std::size_t rcvCount){ return false; };

//*****************************************************************************
//! \brief CDgramDataLink
//!
class CDgramDataLink final : public CBaseDataLink
{
public:
    using CallbackReciveFrom = std::function<bool (EtNet::SPeerAddr ClientAddr, utils::span<char> rx)>;

    CDgramDataLink()                                  = default;
    CDgramDataLink(CDgramDataLink &&rhs)              = default;
    CDgramDataLink& operator=(CDgramDataLink&& rhs)   = default;
    CDgramDataLink(CDgramDataLink const&)             = delete;
    CDgramDataLink& operator=(CDgramDataLink const&)  = delete;

    CDgramDataLink(int socketFd);
    CDgramDataLink(int socketFd, const SPeerAddr &rPeerAddr);

    void sendTo(const SPeerAddr& rClientAddr, const utils::span<char>& rSpanTx);
    void reciveFrom(utils::span<char>& rSpanRx, CallbackReciveFrom scanForEnd);
};

} // EtNet

#endif // _DGRAMDATALINK_H_
