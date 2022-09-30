#pragma once

#include <arpa/inet.h>

void error(char* log,int status_code);

struct Server{
    char* ip;
    int port;
    int domain;
    int type;
    int protocol;
    int MAX_CONNEXTIONS;
    struct sockaddr_in address;

    int socket;
};

struct Server* serverCreate(
    char* ip,
    int port,
    int max_connections,
    int domain,
    int type,
    int protocol
);

int runserver(struct Server* server,std::string HOST_DIR_REL_PATH,struct Configs* config);
