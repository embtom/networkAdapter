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
#include <stream/StreamDataLink.hpp>

using namespace EtNet;

CStreamDataLink::CStreamDataLink(ESocketMode opMode) : 
    CBaseSocket(opMode)
{ }

CStreamDataLink::CStreamDataLink(int socketFd) :
    CBaseSocket(socketFd)
{ }

void CStreamDataLink::send(const char* buffer, std::size_t len)
{
    std::size_t dataWritten = 0;

    while(dataWritten < len)
    {
        std::size_t put = ::write(getFd(), buffer + dataWritten, len - dataWritten);
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

std::size_t CStreamDataLink::recive(uint8_t* buffer, std::size_t len, Callback scanForEnd)
{
    if (getFd() == 0)
    {
        throw std::logic_error(utils::buildErrorMessage("DataSocket::", __func__, ": accept called on a bad socket object (this object was moved)"));
    }

    std::size_t dataRead  = 0;
    while(dataRead < len)
    {
        // The inner loop handles interactions with the socket.
        std::size_t get = ::read(getFd(), buffer + dataRead, len - dataRead);
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
        if (scanForEnd(dataRead))
        {
            break;
        }
    }

    return dataRead;
}
