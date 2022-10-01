#pragma once

#include <vector>


// can be stored in array of 2. for convenience using struct route
struct Route{
    std::string route;
    std::string route_file_path;
};

struct Configs{
    std::string ip;
    int port;
    std::vector<struct Route> routes;
};
 

struct Configs* parserConfig(std::string content);
std::vector<std::string> parserCss(std::string html);
std::vector<std::string> parserJs(std::string html);
int binderCss(std::string* html,std::vector<std::string> css_files,std::string HOST_DIR);
int binderJs(std::string* html,std::vector<std::string> js_files,std::string HOST_DIR);
int html_preprocessor(std::string* html,std::string HOST_DIR,int status);
int file_reader(std::string file_path,std::string *buffer);
int process_request(char* request,std::string* route);