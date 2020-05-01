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
//! The UdpDataLink is communication layer used by UdpClient and UdpServer
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
    CUdpDataLink(CUdpDataLink &&rhs) noexcept;
    CUdpDataLink& operator=(CUdpDataLink&& rhs) noexcept;
    virtual ~CUdpDataLink() noexcept;

    //! The UdpDataLink is instantiated either by UdpServer or UdpClient
    CUdpDataLink(int socketFd);
    CUdpDataLink(int socketFd, const SPeerAddr &rPeerAddr);

    //! data to transmit is passed by a the non-owning span view of type "const uint8_t"
    //! The peer adress to transmit is specified at constrution
    //! e.g uint8_t txData[5] = {0,1,2,3,4};
    //! e.g send(utils::span(txData));
    void send(const utils::span<const uint8_t>& rSpanTx) const;

    //! data to transmit is passed by a the non-owning span view of any type T.
    //! The peer adress to transmit is specified at constrution
    template<typename T, std::enable_if_t<!std::is_same_v<utils::remove_cvref_t<T>,uint8_t>,int> = 0>
    void send(const utils::span<T>& rTxSpan) const {
        send(rTxSpan.as_byte());
    }

    //! data to transmit is passed ty NetOrder reflection helper. It converts the data to transmit
    //! automatically into Network-byte-order (Big Endian).
    //! The peer adress to transmit is specified at constrution
    //! For reflection of the passed type a registration of the members (EtEndian::registerMembers<T>)
    //! is required. See at examples
    template<typename T>
    void send(const EtEndian::CNetOrder<T>& rTx)
    {
        utils::span<std::add_const_t<std::remove_reference_t<T>>> txSpan (rTx.NetworkOrder());
        send(txSpan.as_byte());
    }

    //! data to transmit is passed by a the non-owning span view of type "const uint8_t"
    //! The ClientAddr specifies the peer adress to transmit.
    //! e.g uint8_t txData[5] = {0,1,2,3,4};
    //! e.g send(utils::span(txData));
    void sendTo(const SPeerAddr& rClientAddr, const utils::span<const uint8_t>& rSpanTx) const;

    //! data to transmit is passed by a the non-owning span view of any type T.
    //! The ClientAddr specifies the peer adress to transmit.
    template<typename T, std::enable_if_t<!std::is_same_v<utils::remove_cvref_t<T>,uint8_t>,int> = 0>
    void sendTo(const SPeerAddr& rClientAddr, const utils::span<T>& rTxSpan) const {
        sendTo(rClientAddr, rTxSpan.as_byte());
    }

    //! data to transmit is passed ty NetOrder reflection helper. It converts the data to transmit
    //! automatically into Network-byte-order (Big Endian).
    //! The peer adress to transmit is specified at constrution
    //! For reflection of the passed type a registration of the members (EtEndian::registerMembers<T>)
    //! is required. See at examples
    template<typename T>
    void sendTo(const SPeerAddr& rClientAddr, const EtEndian::CNetOrder<T>& rTx)
    {
        utils::span<std::add_const_t<std::remove_reference_t<T>>> txSpan (rTx.NetworkOrder());
        sendTo(rClientAddr, txSpan.as_byte());
    }

    //! the recive buffer is passed by a the non-owning span view of type "uint8_t"
    //! The return value is Ret::UNBLOCK if "unblockRecive" is called
    ERet reciveFrom(utils::span<uint8_t>& rSpanRx, CallbackReciveFrom scanForEnd = defaultReciveFrom) const;
    ERet reciveFrom(utils::span<uint8_t>&& rSpanRx, CallbackReciveFrom scanForEnd = defaultReciveFrom) const;

    //! the recive buffer is passed by a the non-owning span view of any type T.
    //! The return value is Ret::UNBLOCK if "unblockRecive" is called
    template<typename T, std::enable_if_t<
            utils::is_span_v<utils::remove_cvref_t<T>> &&
            !is_span_uint8_t_v<utils::remove_cvref_t<T>>, int> = 0>
    ERet reciveFrom(T&& rRxSpan, CallbackReciveFrom scanForEnd = defaultReciveFrom) {
        return reciveFrom(rRxSpan.as_byte(), scanForEnd);
    }

    //! data buffer to recive is passed with the assistance of the HostOrder 
    //! reflection helper. It converts the recived data to HostOrder again.
    //! For reflection of the passed type a registration of the members (EtEndian::registerMembers<T>)
    //! is required. See at examples
    //! The return value is Ret::UNBLOCK if "unblockRecive" is called
    template<typename T, std::enable_if_t<EtEndian::is_host_order_v<utils::remove_cvref_t<T>>, int> = 0>
    ERet reciveFrom(T&& rRx, CallbackReciveFrom scanForEnd = defaultReciveFrom)
    {
        using orderType = typename utils::remove_cvref_t<T>::class_type;
        utils::span<orderType> rxSpan(rRx.object());
        return reciveFrom(rxSpan, scanForEnd);
    }

    //The recive methode is blocking if no data is available and can be unblocked.
    bool unblockRecive() noexcept;

private:
    std::unique_ptr<CUdpDataLinkPrivate> m_pPrivate;
};

} // EtNet

#endif // _UDPDATALINK_H_
