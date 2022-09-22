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

    struct Server* server = (struct Server*) malloc(sizeof(struct Server));

    server->ip = ip;
    server->port = port;
    server->MAX_CONNEXTIONS = max_connections;
    server->domain = domain;
    server->type = type;
    server->protocol = protocol;

    memset(&server->address,'\0',sizeof(server->address));
    server->address.sin_family = domain;
    server->address.sin_port = htons(port);
    server->address.sin_addr.s_addr = inet_addr(ip);

    server->socket = socket(domain,type,protocol);
    error(
        "Socket creation failed...\n",
        server->socket
        );

    printf("Socket created.\n");

    
    error(
        "Binding failed...\n",
        bind(server->socket,(struct sockaddr*)&server->address,sizeof(server->address))
        );


    printf("Socket Binded.\n");

    error(
        "Listening Failed..\n",
        listen(server->socket,server->MAX_CONNEXTIONS)
        );


    return server;
}


int runserver(struct Server* server){

    char buffer[50000];
    char* http_response = "HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Type: text/html\nConnection: Closed\n\n<html><body><h1>Hello, saurabh!</h1><br><h1>hi Swapnil</h1></body></html>";
    socklen_t addr_size = sizeof(server->address);


    while(1){
        printf("Waiting for connections...\n");
        int client_socket = accept(server->socket,(struct sockaddr*)&server->address,&addr_size);
        error(
            "Failed to accept connection..\n",
            client_socket
        );
        memset(buffer,'\0',sizeof(buffer));
        read(client_socket,buffer,50000);
        printf("%s\n",buffer);

        write(client_socket,http_response,strlen(http_response));
        printf("message sended\n");
        close(client_socket);
        
    }



    return 0;

}


void error(char* log,int status_code){
    if(status_code<0){
        perror(log);
        exit(1);
    }
}

