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
#include <templateHelpers.h>
#include <HostOrder.h>
#include <NetOrder.h>
#include <NetAdapter.hpp>

#include <iostream>

#include <IpAddress.hpp>

namespace EtNet
{

struct SPeerAddr
{
    CIpAddress Ip;
    unsigned int Port {0};
};

constexpr auto defaultReciveFrom = [](SPeerAddr ClientAddr, utils::span<uint8_t> rx)
{
    std::cout << "DefaultRecive" << std::endl;
    return false; 
};

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

    CUdpDataLink() noexcept;
    CUdpDataLink(CUdpDataLink const&)                    = delete;
    CUdpDataLink& operator=(CUdpDataLink const&)         = delete;
    virtual ~CUdpDataLink() noexcept;

    CUdpDataLink(int socketFd);
    CUdpDataLink(int socketFd, const SPeerAddr &rPeerAddr);
    CUdpDataLink(CUdpDataLink &&rhs) noexcept;
    CUdpDataLink& operator=(CUdpDataLink&& rhs) noexcept;

    void send(const utils::span<const uint8_t>& rSpanTx) const;

    template<typename T, std::enable_if_t<!std::is_same_v<utils::remove_cvref_t<T>,uint8_t>,int> = 0>
    void send(const utils::span<T>& rTxSpan) const {
        send(rTxSpan.as_byte());
    }

    template<typename T>
    void send(const EtEndian::CNetOrder<T>& rTx)
    {
        utils::span<std::add_const_t<std::remove_reference_t<T>>> txSpan (rTx.NetworkOrder());
        send(txSpan.as_byte());
    }

    void sendTo(const SPeerAddr& rClientAddr, const utils::span<const uint8_t>& rSpanTx) const;

    template<typename T, std::enable_if_t<!std::is_same_v<utils::remove_cvref_t<T>,uint8_t>,int> = 0>
    void sendTo(const SPeerAddr& rClientAddr, const utils::span<T>& rTxSpan) const {
        sendTo(rClientAddr, rTxSpan.as_byte());
    }

    template<typename T>
    void sendTo(const SPeerAddr& rClientAddr, const EtEndian::CNetOrder<T>& rTx)
    {
        utils::span<std::add_const_t<std::remove_reference_t<T>>> txSpan (rTx.NetworkOrder());
        sendTo(rClientAddr, txSpan.as_byte());
    }

    ERet reciveFrom(utils::span<uint8_t>& rSpanRx, CallbackReciveFrom scanForEnd = defaultReciveFrom) const;
    ERet reciveFrom(utils::span<uint8_t>&& rSpanRx, CallbackReciveFrom scanForEnd = defaultReciveFrom) const;

    template<typename T, std::enable_if_t<
            utils::is_span_v<utils::remove_cvref_t<T>> &&
            !is_span_uint8_t_v<utils::remove_cvref_t<T>>, int> = 0>
    ERet reciveFrom(T&& rRxSpan, CallbackReciveFrom scanForEnd = defaultReciveFrom) {
        return reciveFrom(rRxSpan.as_byte(), scanForEnd);
    }

    template<typename T, std::enable_if_t<EtEndian::is_host_order_v<utils::remove_cvref_t<T>>, int> = 0>
    ERet reciveFrom(T&& rRx, CallbackReciveFrom scanForEnd = defaultReciveFrom)
    {
        using orderType = typename utils::remove_cvref_t<T>::class_type;
        utils::span<orderType> rxSpan(rRx.object());
        return reciveFrom(rxSpan, scanForEnd);
    }

    bool unblockRecive() noexcept;

private:
    std::unique_ptr<CUdpDataLinkPrivate> m_pPrivate;
};

} // EtNet

#endif // _UDPDATALINK_H_
