#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <vector>
#include "networking/server.h"
#include "utils/utils.h"

int main(int argc, char** argv){
    // if (argc!=2){
    //     perror('Invalid Number of arguments passed');
    //     exit(1);
    // }

    // FILE *server_cnfg = fopen(argv[2],'r');
    // char cnfg_buffer[5000];

    // if (server_cnfg==NULL){
    //     perror('Invalid Config file path');
    //     exit(1);
    // }

    // fscanf(server_cnfg,);

    // struct Server* server = serverCreate(
    //     '192.168.43.84',
    //     80,
    //     5,
    //     AF_INET,
    //     SOCK_STREAM,
    //     0
    // );

    // runserver(server,'');

    std::string html ="<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><link rel=\"stylesheet\" href=\"folder/style.css\"><link rel=\"stylesheet\" href=\"style2.css\"><title>Document</title></head><body></body><script src=\"file33.js\"></script></html>";
    std::vector<std::string> files =  parserJs(html);
    
    for(int i=0;i<files.size();i++){
        std::cout << files[i] << std::endl;
    }

    return 0;
}