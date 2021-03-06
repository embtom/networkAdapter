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

#ifndef _BASESOCKET_H_
#define _BASESOCKET_H_

namespace EtNet
{

enum class ESocketMode
{
    INET_DGRAM,
    INET_STREAM,
    INET6_DGRAM,
    INET6_STREAM,
    NO_MODE
};

//*****************************************************************************
//! \brief CBaseSocket
//! It is encapsulation of the Posix Socket
class CBaseSocket
{
public:
    CBaseSocket() noexcept                     = default;
    CBaseSocket(CBaseSocket const&)            = delete;
    CBaseSocket& operator=(CBaseSocket const&) = delete;

    //! Storage a passed socket at the BaseSocket
    CBaseSocket(int socketFd) noexcept;

    //! Create new socket depending on the operation mode
    CBaseSocket(ESocketMode opMode);

    CBaseSocket(CBaseSocket &&rhs) noexcept;
    CBaseSocket& operator=(CBaseSocket&& rhs) noexcept;
    virtual ~CBaseSocket() noexcept;

    //! Check if socket is valid
    bool isValid() const noexcept;

    //! get containing socket
    int getFd() const noexcept;

    //! request socket domain, AF_INET or AF_INET6
    int getDomain() const noexcept;

    //! apply socketopt SO_REUSEADDR at Basesocket
    static CBaseSocket&& SoReuseSocket(CBaseSocket &&rBaseSocket);

    //! apply socketopt SO_BROADCAST at Basesocket
    static CBaseSocket&& SoBroadcast(CBaseSocket &&rBaseSocket);

private:
    int         m_socketFd{-1};
};

} //EtNet
#endif /*_SOCKET_H_*/