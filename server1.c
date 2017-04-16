//
//  main.c
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
#include <string.h>

#define RCVBUFSIZE 256
#define MAXPENDING 5

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char const *argv[])
{
    FILE * file;
    char * line = NULL;
    int rv_sock, s_sock, port_num, msg_len, count = 0;
    socklen_t clilen;
    size_t len = 0;
    ssize_t read;
    char buffer[RCVBUFSIZE];
    char temp[RCVBUFSIZE];
    char * tmp;
    struct  sockaddr_in serv_addr, cli_addr;
    if (argc != 2)
    {
        char msg[64];
        memset((char *) &msg, 0, 64);
        sprintf(msg, "\nUsage: %s server_port\n", argv[0]);
        error(msg);
    }
    rv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (rv_sock < 0)
    {
        error("ERROR opening socket");
    }
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    port_num = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port_num);
    if (bind(rv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding");
    }
    if (listen(rv_sock, MAXPENDING) < 0)
    {
        error("ERROR on listen");
    }
    clilen = sizeof(cli_addr);
    s_sock = accept(rv_sock, (struct sockaddr *) &cli_addr, &clilen);
    if (s_sock < 0)
    {
        error("ERROR accepting new client");
    }
    printf("\nNew Client connected from port number %d and IP %s\n", ntohs(cli_addr.sin_port), inet_ntoa(cli_addr.sin_addr));
    while(1)
    {
        
        
        memset(buffer, 0, RCVBUFSIZE);
        memset(temp, 0, RCVBUFSIZE);
        count = 0;
        msg_len = recv(s_sock, buffer, RCVBUFSIZE - 1, 0);
        printf("\nReceived command: %s\n", buffer);
        strcpy(temp, buffer);
        tmp = strtok (temp, " ");
        if(tmp == NULL)
        {
            send(s_sock, "IMPROPER COMMAND", 16, 0);
            continue;
        }
        if(strcmp("GET", tmp) == 0)
        {
            tmp = strtok (NULL, "\n");
            printf("%s",tmp );
            if(tmp == NULL)
            {
                memset(buffer, 0, RCVBUFSIZE);
                strcpy(buffer, "No Filename Entered");
                msg_len = send(s_sock, buffer, strlen(buffer), 0);
                continue;
            }
            printf("\nTrying to open FILE: %s\n", tmp);
            file = fopen(tmp, "r");
            if (file)
            {
                while ((read = getline(&line, &len, file)) != -1)
                    printf("%s", line);
                printf("\n");
                fclose(file);
                send(s_sock, "FILE FOUND", 10, 0);
                continue;
            }
            printf("\nInvalid Filename\n");
            send(s_sock, "INVALID FILENAME", 16, 0);
            continue;
        }
        else
        {
            if(strcmp("BOUNCE", tmp) == 0)
            {
                tmp = strtok(NULL, "\n");
                while(tmp != NULL)
                {
                    printf("%s ", tmp);
                    tmp = strtok(NULL, " ");
                    
                }
                printf("\n");
                send(s_sock, "MESSAGE DISPLAYED", 17, 0);
                continue;
            }
            else
            {
                if (strcmp("EXIT\n", tmp) == 0 || strcmp("EXIT", tmp) == 0)
                {	
                    printf("\nEXITING..  ");
                    send(s_sock, "Client Disconnecting", 21, 0);
                    tmp = strtok(NULL, "\n");
                    while(tmp != NULL)
                    {	
                        printf("%s ",tmp);
                        tmp = strtok(NULL, " ");
                    }
                    printf("\nClient Disconnected\n");
                    return 0;
                }
                else
                {
                    printf("\nImproper command\n");
                    send(s_sock, "IMPROPER COMMAND", 16, 0);
                    close(s_sock);
                    close(rv_sock);
                    continue;
                }
            }
        }
    }
    close(rv_sock);
    close(s_sock);
    printf("\nClient Disconnected\n");
    return 0;
}