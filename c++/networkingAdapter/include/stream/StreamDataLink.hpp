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


#ifndef _DATASOCKET_H_
#define _DATASOCKET_H_

#include <cstddef>
#include <functional>
#include <BaseSocket.hpp>
#include <stdint.h>

namespace EtNet
{
    using Callback = std::function<bool (std::size_t len)>;

    auto defaultNoScanForEnd = [](std::size_t rcvCount){ return false; };

    class CStreamDataLink : public  CBaseSocket
    {

        public:
        CStreamDataLink(CStreamDataLink &&rhs)              = default;
        CStreamDataLink& operator=(CStreamDataLink&& rhs)   = default;
        CStreamDataLink(CStreamDataLink const&)             = delete;
        CStreamDataLink& operator=(CStreamDataLink const&)  = delete;
        CStreamDataLink()                               = default;

        CStreamDataLink(ESocketMode opMode);
        CStreamDataLink(int socketFd);

        std::size_t recive(uint8_t* buffer, std::size_t len, Callback scanForEnd = defaultNoScanForEnd);
        void        send(const char* buffer, std::size_t len);
    };

}

#endif /* _DATASOCKET_H_ */
