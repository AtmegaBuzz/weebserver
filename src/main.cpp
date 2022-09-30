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

    // config file parser 
    std::string BASE_DIR = std::filesystem::current_path();
    std::string host_dir_rel_path = argv[2];

    std::ifstream config_file(BASE_DIR+"/"+host_dir_rel_path+"/"+".config"); 
    std::string config_content;
    char character;

    while(config_file>>character){
        config_content+=character;
    }

    struct Configs* config = parserConfig(config_content);

    struct Server* server = serverCreate(
        (char*)config->ip.c_str(),
        config->port,
        5,
        AF_INET,
        SOCK_STREAM,
        0
    );

    runserver(server,host_dir_rel_path,config);

    return 0;
}