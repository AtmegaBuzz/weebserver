#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/socket.h>
#include <vector>
#include <filesystem>
#include <string>
#include "networking/server.h"
#include "utils/utils.h"

int main(int argc, char** argv){
    if (argc!=3){
        perror("Invalid Number of arguments passed");
        exit(1);
    } 

    struct Server* server = serverCreate(
        "192.168.0.5",
        80,
        5,
        AF_INET,
        SOCK_STREAM,
        0
    );


    std::string host_dir_rel_path = argv[2];
    runserver(server,"",host_dir_rel_path);



    return 0;
}