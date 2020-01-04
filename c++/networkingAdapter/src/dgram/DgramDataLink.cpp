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

#include <stdexcept>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <error_msg.hpp>
#include <dgram/DgramDataLink.hpp>

using namespace EtNet;

CDgramDataLink::CDgramDataLink(ESocketMode opMode) : 
    CBaseSocket(opMode)
{ }

CDgramDataLink::CDgramDataLink(int socketFd) :
    CBaseSocket(socketFd)
{ }

void CDgramDataLink::sendTo(const SClientAddr& rClientAddr, const char* buffer, std::size_t len)
{
    sockaddr_in clAddr{};
    sockaddr_in6 clAddr6{};
    sockaddr* claddr;
    socklen_t claddrLen;

    if (rClientAddr.Ip.is_v4())
    {
        clAddr.sin_family       = AF_INET;
        clAddr.sin_port         = rClientAddr.Port;
        std::memcpy(&clAddr.sin_addr, rClientAddr.Ip.to_v4(), sizeof(in_addr));
        claddr = (sockaddr*)&clAddr;
        claddrLen = sizeof(sockaddr_in);
    }
    else if (rClientAddr.Ip.is_v6())
    {   
        clAddr6.sin6_family      = AF_INET6;
        clAddr6.sin6_port        = rClientAddr.Port;
        std::memcpy(&clAddr6.sin6_addr, rClientAddr.Ip.to_v6(), sizeof(in6_addr));
        claddr = (sockaddr*)&clAddr6;
        claddrLen = sizeof(sockaddr_in6);
    }
    else {
        throw std::logic_error(utils::buildErrorMessage("CDgramServer::", __func__, " : No valid Ip to connect"));
    }

    std::size_t dataWritten = 0;
    while(dataWritten < len)
    {
        std::size_t put = ::sendto(getFd(), buffer + dataWritten, len - dataWritten, 0, claddr, claddrLen);
        if (put == static_cast<std::size_t>(-1))
        {
            switch(errno)
            {
                case EINVAL:     [[fallthrough]];
                case EBADF:      [[fallthrough]];
                case ECONNRESET: [[fallthrough]];
                case ENXIO:      [[fallthrough]];
                case EPIPE:
                {
                    // Fatal error
                    throw std::domain_error(utils::buildErrorMessage("DataSocket::", __func__, ": write: critical error: ", strerror(errno)));
                }
                case EDQUOT:     [[fallthrough]];
                case EFBIG:      [[fallthrough]];
                case EIO:        [[fallthrough]];                    
                case ENETDOWN:   [[fallthrough]];
                case ENETUNREACH:[[fallthrough]];
                case ENOSPC:
                {
                    // Resource acquisition failure or device error
                    throw std::runtime_error(utils::buildErrorMessage("DataSocket::", __func__, ": write: resource failure: ", strerror(errno)));
                }
                case EINTR:      [[fallthrough]];
                        // TODO: Check for user interrupt flags.
                        //       Beyond the scope of this project
                        //       so continue normal operations.
                case EAGAIN:
                { 
                    // Temporary Error, retry the read.
                    continue;
                }
                default:
                {
                    throw std::runtime_error(utils::buildErrorMessage("DataSocket::", __func__, ": write: returned -1: ", strerror(errno)));
                }
            }
        }
        dataWritten += put;
    }
    return;
}

std::size_t CDgramDataLink::reciveFrom(uint8_t* buffer, std::size_t len, Callback scanForEnd)
{
    union e
    {   
        sockaddr_storage  common;
        sockaddr_in       sin;
        sockaddr_in6      sin6;
    } peerAdr;
    socklen_t addr_size = sizeof(peerAdr);

    SClientAddr peerAddress;
    auto toCIpAddress = [&peerAddress] (e& peerAdr) 
    {
        switch (peerAdr.common.ss_family)
        {
            case AF_INET:
            {
                peerAddress.Ip = std::move(CIpAddress(peerAdr.sin.sin_addr));
                peerAddress.Port = peerAdr.sin.sin_port;
                break;
            }
            case AF_INET6:
            { 
                if (IN6_IS_ADDR_V4MAPPED(&peerAdr.sin6.sin6_addr)) {
                    in_addr ip4;
                    std::memcpy(&ip4, &peerAdr.sin6.sin6_addr.__in6_u.__u6_addr8[12], sizeof(in_addr));
                    peerAddress.Ip = std::move(CIpAddress(ip4));
                }
                else {
                    peerAddress.Ip = std::move(CIpAddress(peerAdr.sin6.sin6_addr));    
                }
                peerAddress.Port = peerAdr.sin6.sin6_port;
                break;
            }
        }
    };
    uint8_t* readBuffer = buffer;
    std::size_t dataRead  = 0;

    while(dataRead < len)
    {
        // The inner loop handles interactions with the socket.
        std::size_t get = ::recvfrom(getFd(), readBuffer + dataRead, len - dataRead, 0, (sockaddr*)&peerAdr, &addr_size);
        if (get == static_cast<std::size_t>(-1))
        {
            switch(errno)
            {
                case EBADF:     [[fallthrough]];
                case EFAULT:    [[fallthrough]];
                case EINVAL:    [[fallthrough]];
                case ENXIO:
                {
                    // Fatal error. Programming bug
                    throw std::domain_error(utils::buildErrorMessage("DataSocket::", __func__, ": read: critical error: ", strerror(errno)));
                }

                case EIO:       [[fallthrough]];
                case ENOBUFS:   [[fallthrough]];
                case ENOMEM:
                {
                   // Resource acquisition failure or device error
                    throw std::runtime_error(utils::buildErrorMessage("DataSocket::", __func__, ": read: resource failure: ", strerror(errno)));
                }
                case EINTR:     [[fallthrough]];
                    // TODO: Check for user interrupt flags.
                    //       Beyond the scope of this project
                    //       so continue normal operations.
                case ETIMEDOUT: [[fallthrough]];
                case EAGAIN:
                {
                    // Temporary error, retry
                    continue;
                }
                case ECONNRESET:[[fallthrough]];
                case ENOTCONN:
                {
                    // Connection broken.
                    // Return the data we have available and exit
                    // as if the connection was closed correctly.
                    get = 0;
                    break;
                }
                default:
                {
                    throw std::runtime_error(utils::buildErrorMessage("DataSocket::", __func__, ": read: returned -1: ", strerror(errno)));
                }
            }
        }
        if (get == 0)
        {
            break;
        }
        dataRead += get;
        
        toCIpAddress(peerAdr);
        if (scanForEnd(peerAddress, dataRead))
        {
            break;
        }
    }
    return dataRead;
}

