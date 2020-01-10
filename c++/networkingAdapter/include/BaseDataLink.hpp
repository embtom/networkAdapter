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

#ifndef _BASEDATALINK_H_
#define _BASEDATALINK_H_

//******************************************************************************
// Header
#include <cstddef>
#include <functional>
#include <BaseSocket.hpp>
#include <stdint.h>

namespace EtNet
{
constexpr auto defaultOneRead = [](std::size_t rcvCount){ return true; };

//*****************************************************************************
//! \brief CBaseDataLink
//!
class CBaseDataLink
{
public:
    using CallbackReceive = std::function<bool (std::size_t len)>;
    
    CBaseDataLink(CBaseDataLink &&rhs) noexcept;
    CBaseDataLink& operator=(CBaseDataLink&& rhs) noexcept;
    CBaseDataLink(CBaseDataLink const&)             = delete;
    CBaseDataLink& operator=(CBaseDataLink const&)  = delete;
    CBaseDataLink()                                 = default;

    virtual ~CBaseDataLink()                        = default;

    std::size_t recive(uint8_t* buffer, std::size_t len, CallbackReceive scanForEnd = defaultOneRead);
    void        send(const char* buffer, std::size_t len);

protected:

    CBaseDataLink(int socketFd) noexcept;
    int getFd() const noexcept;
    void closeFd() noexcept;

private:
    int m_socketFd {-1};
};


}


#endif // _BASEDATALINK_H_