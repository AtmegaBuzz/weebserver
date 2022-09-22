#include <stdio.h>
#include <sys/socket.h>
#include "networking/server.h"


int main(){

    struct Server* server = serverCreate(
        "192.168.43.84",
        80,
        5,
        AF_INET,
        SOCK_STREAM,
        0
    );

    runserver(server);

    return 0;
}