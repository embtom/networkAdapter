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

#ifndef _DGRAMSERVER_H_
#define _DGRAMSERVER_H_

#include <functional>
#include <IpAddress.hpp>
#include <BaseSocket.hpp>
#include <dgram/DgramDataLink.hpp>
#include <memory>

namespace EtNet
{




// struct SClientAddr
// {
//     CIpAddress Ip;
//     unsigned int Port;
// };
class CDgramServerPrivate;




class CDgramServer
{
public:
    using Callback = std::function<bool (EtNet::SClientAddr ClientAddr, std::size_t len)>;
    
    CDgramServer(CBaseSocket&& rhs, int port);

    CDgramServer(const CDgramServer&)             = delete;
    CDgramServer& operator= (const CDgramServer&) = delete;
    CDgramServer(CDgramServer&&)                  = default;
    CDgramServer& operator= (CDgramServer&&)      = default;
    CDgramServer()                                = default;

    ~CDgramServer();
    CDgramDataLink waitForConnection();

    void sendTo(const SClientAddr& rClientAddr, const char* buffer, std::size_t len);
    std::size_t reciveFrom(uint8_t* buffer, std::size_t len, Callback scanForEnd);

private:
    static void privateDeleterHook(CDgramServerPrivate *it);
   
    struct privateDeleter
    {
        void operator()(CDgramServerPrivate *it) {
            privateDeleterHook(it);
        }
    };

    std::unique_ptr<CDgramServerPrivate,privateDeleter> m_pPrivate;
};

} // EtNet
#endif // _DGRAMCLIENT_H_
