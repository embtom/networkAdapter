/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 59-3 */

/* i6d_ucase_sv.c

   A server that receives datagrams, converts their contents to uppercase, and
   then returns them to the senders.

   See also i6d_ucase_cl.c.
*/
#include <iostream>
#include <dgram/DgramServer.hpp>
#include <thread>
#include "i6d_ucase.h"

#define PORT_NUM 50002    

int main()
{
    auto baseSocket = EtNet::CBaseSocket(EtNet::ESocketMode::INET_DGRAM);
    EtNet::CDgramServer DgramServer(std::move(baseSocket), PORT_NUM);

    while (true)
    {
        uint8_t buffer [128];
        int rcvLen;
        EtNet::SClientAddr addr;

        rcvLen = DgramServer.reciveFrom(buffer, sizeof(buffer), [&addr, &buffer, &DgramServer](EtNet::SClientAddr ClientAddr, std::size_t len) 
        {
            std::cout << "Message form: " << addr.Ip.toString() << " with length: " << len << std::endl;
            addr = std::move(ClientAddr);
            for (int j = 0; j < len; j++) {
                buffer[j] = toupper((unsigned char) buffer[j]);
            }
            return true;
        });

        DgramServer.sendTo(addr,(char*)buffer,rcvLen);
    }
}




//#define INADDR_ANY ((unsigned long int) 0x00000000)

// int main(int argc, char *argv[])
// {
//     struct sockaddr_in svaddr, claddr;
//     int j;
//     ssize_t numBytes;
//     socklen_t len;
//     char buf[BUF_SIZE];
//     char claddrStr[INET6_ADDRSTRLEN];

//     EtNet::CDataSocket dataSocket(EtNet::ESocketMode::INET_DGRAM);

//     memset(&svaddr, 0, sizeof(struct sockaddr_in));
//     svaddr.sin_family = AF_INET;
//     svaddr.sin_addr.s_addr = INADDR_ANY; /* Wildcard address */
//     svaddr.sin_port = htons(PORT_NUM);

//     if (bind(dataSocket.getFd(), (struct sockaddr *)&svaddr, sizeof(struct sockaddr)) == -1)
//         errExit("bind");

//     for (;;)
//     {
//         len = sizeof(struct sockaddr_in);

//         numBytes = recvfrom(dataSocket.getFd(), buf, BUF_SIZE, 0, (struct sockaddr *)&claddr, &len);
//         if (numBytes == -1)
//             errExit("recvfrom");

//         /* Display address of client that sent the message */

//         if (inet_ntop(AF_INET, &claddr.sin_addr, claddrStr, INET6_ADDRSTRLEN) == NULL)
//             printf("Couldn't convert client address to string\n");
//         else
//             printf("Server received %ld bytes from (%s, %u)\n",
//                    (long)numBytes, claddrStr, ntohs(claddr.sin_port));

//         for (j = 0; j < numBytes; j++)
//             buf[j] = toupper((unsigned char)buf[j]);

//         if (sendto(dataSocket.getFd(), buf, numBytes, 0, (struct sockaddr *)&claddr, len) != numBytes)
//             fatal("sendto");
//     }
// }
