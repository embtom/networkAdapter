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

#ifndef _TCPDATALINK_H_
#define _TCPDATALINK_H_

//******************************************************************************
// Header

#include <stdint.h>
#include <cstddef>
#include <functional>
#include <memory>
#include <span.h>

namespace EtNet
{

constexpr auto defaultOneRead = [](utils::span<char> rx){ return true; };

class CTcpDataLinkPrivate;

//*****************************************************************************
//! \brief CTcpDataLink
//!
class CTcpDataLink
{
public:
    enum class ERet
    {
        OK,
        UNBLOCK
    };

    using CallbackReceive = std::function<bool (utils::span<char> rx)>;

    CTcpDataLink() noexcept                              = default;
    CTcpDataLink(CTcpDataLink const&)                    = default;
    CTcpDataLink& operator=(CTcpDataLink const&)         = default;
    virtual ~CTcpDataLink() noexcept;

    CTcpDataLink(int socketFd) noexcept;
    CTcpDataLink(CTcpDataLink &&rhs) noexcept;
    CTcpDataLink& operator=(CTcpDataLink&& rhs) noexcept;

    void send(const utils::span<char>& rTxSpan);

    bool unblockRecive() noexcept;
    CTcpDataLink::ERet recive(utils::span<char>& rRxSpan, CallbackReceive scanForEnd = defaultOneRead);

private:
    std::shared_ptr<CTcpDataLinkPrivate> m_pPrivate;
};

}

#endif // _TCPDATALINK_H_
