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

#include <algorithm>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <error_msg.hpp>
#include <BaseSocket.hpp>

using namespace EtNet;

CBaseSocket::CBaseSocket(CBaseSocket &&rhs) noexcept
{
    std::swap(m_socketFd, rhs.m_socketFd);
}

CBaseSocket& CBaseSocket::operator=(CBaseSocket&& rhs) noexcept
{
    std::swap(m_socketFd, rhs.m_socketFd);
    return *this;
}

CBaseSocket::CBaseSocket(ESocketMode opMode)
{
    int ret;
    switch (opMode)
    {
        case ESocketMode::INET_DGRAM:
        {
            ret = ::socket(AF_INET, SOCK_DGRAM, 0);
            break;
        }
        case ESocketMode::INET_STREAM:
        {
            ret = ::socket(AF_INET, SOCK_STREAM, 0);
            break;
        }
        case ESocketMode::INET6_DGRAM:
        {
            ret = ::socket(AF_INET6, SOCK_DGRAM, 0);
            break;
        }
        case ESocketMode::INET6_STREAM:
        {
            ret = ::socket(AF_INET6, SOCK_STREAM, 0);
            break;
        }
    }

    if (ret < 0) {
        throw std::runtime_error(utils::buildErrorMessage("CBaseSocket::", __func__, ": bad socket: ", strerror(errno)));
    }
    m_socketFd = ret;
}

CBaseSocket::CBaseSocket(int socketFd) :
    m_socketFd(socketFd)
{
    if (socketFd <= 0)
    {
        throw std::runtime_error(utils::buildErrorMessage("CBaseSocket::", __func__, ": bad socket: ", strerror(errno)));
    }
}

CBaseSocket::~CBaseSocket()
{
    if (m_socketFd > 0) {
        ::close(m_socketFd);
        m_socketFd=0;
    }
}

int CBaseSocket::getFd() const
{
    return m_socketFd;
}

int CBaseSocket::getDomain() const noexcept
{
    int domain;
    socklen_t length = sizeof(int);
    if(getsockopt(m_socketFd, SOL_SOCKET, SO_DOMAIN, &domain, &length) == -1) {
        return -1;
    }
    return domain;
}

CBaseSocket& CBaseSocket::SoReuseSocket(CBaseSocket &rBaseSocket)
{
    int reuseaddr = 1;
    setsockopt(rBaseSocket.getFd(), SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
    return rBaseSocket;
}

