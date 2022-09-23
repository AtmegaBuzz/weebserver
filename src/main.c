#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "networking/server.h"


int main(int argc, char** argv){
    if (argc!=2){
        perror("Invalid Number of arguments passed");
        exit(1);
    }

    FILE *server_cnfg = fopen(argv[2],"r");

    if (server_cnfg==NULL){
        perror("Invalid Config file path");
        exit(1);
    }

    
;
    // struct Server* server = serverCreate(
    //     "192.168.43.84",
    //     80,
    //     5,
    //     AF_INET,
    //     SOCK_STREAM,
    //     0
    // );

    // runserver(server);

    return 0;
}