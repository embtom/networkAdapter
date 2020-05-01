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

#ifndef _TCPDATALINK_H_
#define _TCPDATALINK_H_

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


namespace EtNet
{

constexpr auto defaultOneRead = [](utils::span<uint8_t> rx){ return true; };

class CBaseSocket;
class CTcpDataLinkPrivate;

//*****************************************************************************
//! \brief CTcpDataLink
//! The TcpDataLink is communication layer used by TcpClient and TcpServer
class CTcpDataLink
{
public:
    enum class ERet
    {
        OK,
        UNBLOCK
    };

    using CallbackReceive = std::function<bool (utils::span<uint8_t> rx)>;

    CTcpDataLink() noexcept                              = default;
    CTcpDataLink(CTcpDataLink const&)                    = default;
    CTcpDataLink& operator=(CTcpDataLink const&)         = default;
    CTcpDataLink(CTcpDataLink &&rhs) noexcept;
    CTcpDataLink& operator=(CTcpDataLink&& rhs) noexcept;
    virtual ~CTcpDataLink() noexcept;

    //! The TcpDataLink is instantiated either by TcpServer or TcpClient
    CTcpDataLink(int socketFd) noexcept;
    CTcpDataLink(CBaseSocket&& rBaseSocket) noexcept;

    //! data to transmit is passed by a the non-owning span view of type "const uint8_t"
    //! e.g uint8_t txData[5] = {0,1,2,3,4};
    //! e.g send(utils::span(txData));
    void send(const utils::span<const uint8_t>& rTxSpan) const;

    //! data to transmit is passed by a the non-owning span view of any type T.
    template<typename T, std::enable_if_t<!std::is_same_v<utils::remove_cvref_t<T>,uint8_t>,int> = 0>
    void send(const utils::span<T>& rTxSpan) const {
        send(rTxSpan.as_byte());
    }

    //! data to transmit is passed ty NetOrder reflection helper. It converts the data to transmit
    //! automatically into Network-byte-order (Big Endian).
    //! For reflection of the passed type a registration of the members (EtEndian::registerMembers<T>)
    //! is required. See at examples
    template<typename T>
    void send(const EtEndian::CNetOrder<T>& rTx)
    {
        utils::span<std::add_const_t<std::remove_reference_t<T>>> txSpan (rTx.NetworkOrder());
        send(txSpan.as_byte());
    }

    //! the recive buffer is passed by a the non-owning span view of type "uint8_t"
    //! The return value is Ret::UNBLOCK if "unblockRecive" is called
    ERet recive(utils::span<uint8_t>& rRxSpan, CallbackReceive scanForEnd = defaultOneRead);
    ERet recive(utils::span<uint8_t>&& rRxSpan, CallbackReceive scanForEnd = defaultOneRead);

    //! the recive buffer is passed by a the non-owning span view of any type T.
    //! The return value is Ret::UNBLOCK if "unblockRecive" is called
    template<typename T, std::enable_if_t<
            utils::is_span_v<utils::remove_cvref_t<T>> &&
            !is_span_uint8_t_v<utils::remove_cvref_t<T>>, int> = 0>
    ERet recive(T&& rRxSpan, CallbackReceive scanForEnd = defaultOneRead)
    {
        return recive(rRxSpan.as_byte(), scanForEnd);
    }

    //! data buffer to recive is passed with the assistance of the HostOrder 
    //! reflection helper. It converts the recived data to HostOrder again.
    //! For reflection of the passed type a registration of the members (EtEndian::registerMembers<T>)
    //! is required. See at examples
    //! The return value is Ret::UNBLOCK if "unblockRecive" is called
    template<typename T, std::enable_if_t<EtEndian::is_host_order_v<utils::remove_cvref_t<T>>, int> = 0>
    ERet recive(T&& rRx, CallbackReceive scanForEnd = defaultOneRead)
    {
        using orderType = typename utils::remove_cvref_t<T>::class_type;
        utils::span<orderType> rxSpan(rRx.object());
        return recive(rxSpan, scanForEnd);
    }

    //The recive methode is blocking if no data is available and can be unblocked.
    bool unblockRecive() noexcept;
private:
    std::shared_ptr<CTcpDataLinkPrivate> m_pPrivate;
};

}

#endif // _TCPDATALINK_H_
