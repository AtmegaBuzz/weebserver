#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "server.h"
#include "../utils/utils.h"

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

    printf("Listening For Connections.\n");

    return server;
}


int handle_client(std::string HOST_DIR_REL_PATH,std::string* buffer,struct Configs* config,std::string route_filename){

    std::string BASE_DIR = std::filesystem::current_path();
    std::string DIR = HOST_DIR_REL_PATH;
    std::string HOST_DIR = BASE_DIR + "/" + DIR;

    std::string* html = buffer;
    if(file_reader(HOST_DIR +"/"+route_filename,html)){
        std::cout << "500 internal server error" << std::endl;

        file_reader(BASE_DIR+"/"+"responses/500.html",html)
        html_preprocessor(html,HOST_DIR);
        return 1;
    }
    else{
        html_preprocessor(html,HOST_DIR);
        return 0;
    }
        
}

int request_router(struct Configs* config,std::string route,std::string* route_file_name){
    
    for(int i=0;i<config->routes.size();i++){
        if(config->routes[i].route==("/"+route)){
            *route_file_name = config->routes[i].route_file_path;
            return 0;
        }
    }

    // find 404 error file to show 404 err
    *route_file_name = "responses/404.html";

    return 1;

}


int runserver(struct Server* server,std::string HOST_DIR_REL_PATH,struct Configs* config){

    char buffer[50000];
    std::string html_response_buff;
    socklen_t addr_size = sizeof(server->address);


    while(1){
        printf("Waiting for connections...\n");
        int client_socket = accept(server->socket,(struct sockaddr*)&server->address,&addr_size);
        error(
            "Failed to accept connection..\n",
            client_socket
        );
        memset(buffer,'\0',sizeof(buffer));
        html_response_buff = "";

        read(client_socket,buffer,50000);

        std::string route;
        std::string route_filename;
        process_request(buffer,&route);

        request_router(config,route,&route_filename);

        handle_client(HOST_DIR_REL_PATH,&html_response_buff,config,route_filename);
        write(client_socket,html_response_buff.c_str(),strlen(html_response_buff.c_str()));
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

