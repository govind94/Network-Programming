//
//  client1.c
//  Network_Programming
//
//  Created by Govindan Vedanarayanan on 10/04/17.
//  Copyright © 2017 Govindan Vedanarayanan. All rights reserved.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdarg.h>

#define RCVBUFSIZE 256

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char const *argv[])
{
    int c_sock, port_num, msg_len;
    char buffer[RCVBUFSIZE];
    struct sockaddr_in serv_addr;
    struct hostent *serverIP;
    if (argc != 3)
    {
        char msg[64];
        memset((char *) &msg, 0, 64); /* erase */
        sprintf(msg, "\nUsage: %s serv_name serv_port\n", argv[0]);
        error(msg);
    }
    port_num = atoi(argv[2]);
    c_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (c_sock < 0)
    {
        error("ERROR opening socket");
    }
    serverIP = gethostbyname(argv[1]);
    if(serverIP == NULL)
    {
        error("ERROR, server host name unknown");
    }
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    //bcopy((char *)serverIP->h_addr, (char *)&serv_addr.sin_addr.s_addr, serverIP->h_length);
    memcpy((char *) &(serverIP->h_addr), (char *)&serv_addr.sin_addr.s_addr, serverIP->h_length);
    serv_addr.sin_port = htons(port_num);
    if (connect(c_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR connecting");
    }
    while(1)
    {
        printf("Enter command: ");
        memset(buffer, 0, RCVBUFSIZE);
        fgets(buffer, RCVBUFSIZE, stdin);
        msg_len = send(c_sock, buffer, strlen(buffer), 0);
        if(msg_len < 0)
        {
            error("ERROR writing to socket");
        }
        memset(buffer, 0, RCVBUFSIZE);
        msg_len = recv(c_sock, buffer, RCVBUFSIZE - 1, 0);
        if(msg_len < 0)
        {
            error("ERROR reading from socket");
        }
        fprintf(stdout, "\nServer says: %s\n", buffer);
        if(strstr(buffer, "Client Disconnecting") != 0)
        {
            close(c_sock);
            return 0;
        }
    }
    return 0;
}
