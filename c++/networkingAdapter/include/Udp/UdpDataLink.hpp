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
 * THE SOFTWARE IS PROVIDED "AS IS", WITUdpClientHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _UDPDATALINK_H_
#define _UDPDATALINK_H_

//******************************************************************************
// Header

#include <stdint.h>
#include <cstddef>
#include <functional>
#include <memory>
#include <span.h>
#include <BaseSocket.hpp>
#include <IpAddress.hpp>

namespace EtNet
{

struct SPeerAddr
{
    CIpAddress Ip;
    unsigned int Port {0};
};

constexpr auto defaultReciveFrom = [](SPeerAddr ClientAddr, std::size_t rcvCount){ return false; };

class CUdpDataLinkPrivate;
//*****************************************************************************
//! \brief CUdpDataLink
//!
class CUdpDataLink
{
public:
    enum class ERet
    {
        OK,
        UNBLOCK
    };

    using CallbackReciveFrom = std::function<bool (EtNet::SPeerAddr ClientAddr, utils::span<uint8_t> rx)>;

    CUdpDataLink(CUdpDataLink const&)                    = delete;
    CUdpDataLink& operator=(CUdpDataLink const&)         = delete;

    CUdpDataLink() noexcept;
    CUdpDataLink(int socketFd);
    CUdpDataLink(int socketFd, const SPeerAddr &rPeerAddr);
    CUdpDataLink(CUdpDataLink &&rhs) noexcept;
    CUdpDataLink& operator=(CUdpDataLink&& rhs) noexcept;
    virtual ~CUdpDataLink() noexcept;

    void send(const utils::span<uint8_t>& rSpanTx) const;
    void sendTo(const SPeerAddr& rClientAddr, const utils::span<uint8_t>& rSpanTx) const;

    bool unblockRecive() noexcept;
    ERet reciveFrom(utils::span<uint8_t>& rSpanRx, CallbackReciveFrom scanForEnd) const;

private:
    std::unique_ptr<CUdpDataLinkPrivate> m_pPrivate;
};

} // EtNet

#endif // _UDPDATALINK_H_
