/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 59-4 */

/* i6d_ucase_cl.c

   Client for i6d_ucase_sv.c: send each command-line argument as a datagram to
   the server, and then display the contents of the server's response datagram.
*/
#include <BaseSocket.hpp>
#include <stream/StreamDataLink.hpp>
#include "i6d_ucase.h"
#undef min
#undef max

int
main(int argc, char *argv[])
{
    EtNet::CStreamDataLink baseSocket(EtNet::ESocketMode::INET_DGRAM);

    struct sockaddr_in svaddr;
    int  j;
    size_t msgLen;
    ssize_t numBytes;
    char resp[BUF_SIZE];

    // if (argc < 3 || strcmp(argv[1], "--help") == 0)
    //     usageErr("%s host-address msg...\n", argv[0]);

    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    svaddr.sin_port = htons(PORT_NUM);

    //inet_pton(AF_INET, "127.0.0.1", &svaddr.sin_addr);

    if (inet_pton(AF_INET, argv[1], &svaddr.sin_addr) <= 0)
       fatal("inet_pton failed for address '%s'", argv[1]);

    /* Send messages to server; echo responses on stdout */
    connect(baseSocket.getFd(), (struct sockaddr*)&svaddr, sizeof(struct sockaddr_in));

    for (j = 2; j < argc; j++) {
        msgLen = strlen(argv[j]);

        baseSocket.send(argv[j], msgLen);

        auto ld = [](std::size_t len) { 
            printf("Size %u", (unsigned int)len);
            return true;
        };

       // numBytes = recvfrom(baseSocket.getFd(), resp, BUF_SIZE, 0, NULL, NULL);
    
        numBytes = baseSocket.recive((uint8_t*)resp,BUF_SIZE, [](std::size_t len) { 
            printf("Rcv Len %u\n", (unsigned int)len);
            return true;
        });

        printf("Response %d: %.*s\n", j - 1, (int) numBytes, resp);
    }

    exit(EXIT_SUCCESS);
}

        // numBytes = recvfrom(baseSocket.getFd(), resp, BUF_SIZE, 0, NULL, NULL);
        // if (numBytes == -1)
        //     errExit("recvfrom");

        // if (write(baseSocket.getFd(), argv[j], msgLen) != msgLen)
        // {
        //     fatal("send");
        // }
        // // if (sendto(sfd, argv[j], msgLen, 0, (struct sockaddr *) &svaddr,
        //             sizeof(struct sockaddr_in)) != msgLen)
        //     fatal("sendto");
