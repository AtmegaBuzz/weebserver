#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "server.h"


struct Server* serverCreate(
    char* ip,
    int port,
    int max_connections,
    int domain,
    int type,
    int protocol
){

    struct Server server;

    server.ip = ip;
    server.port = port;
    server.MAX_CONNEXTIONS = max_connections;
    server.domain = domain;
    server.type = type;
    server.protocol = protocol;

    memset(&server.address,'\0',sizeof(server.address));
    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = inet_addr(ip);

    server.socket = socket(domain,type,protocol);
    error(
        "Socket creation failed...\n",
        server.socket
        );

    printf("Socket created.\n");

    
    error(
        "Binding failed...\n",
        bind(server.socket,(struct sockaddr*)&server.address,sizeof(server.address))
        );


    printf("Socket Binded.\n");

    error(
        "Listening Failed..\n",
        listen(server.socket,server.MAX_CONNEXTIONS)
        );


    return &server;
}


int runserver(struct Server* server){

    char buffer[50000];

    while(1){
        printf("Waiting for connections...\n");

        int client_socket = accept(server->socket,(struct sockaddr*)&server->address,(socklen_t *)sizeof(server->address));
        error(
            "Failed to accept connection..\n",
            client_socket
        );
        memset(buffer,'\0',sizeof(buffer));
        read(client_socket,buffer,50000);
        printf("%s\n",buffer);


    }



    return 0;

}


void error(char* log,int status_code){
    if(status_code<0){
        perror(log);
        exit(1);
    }
}

