#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include "utils.h"



int file_reader(std::string file_path,std::string *buffer){

    std::ifstream read_file(file_path);
    *buffer = ""; // reiniting the buffer
    
    if(read_file.is_open()){

        char chunk;

        while(read_file.get(chunk)){
            *buffer += chunk;
        }

        read_file.close();
        return 0;
    }
    else{
        return 1;
    }


}

std::vector<std::string> split(std::string str,char delimiter){

    std::vector<std::string> splitted_str = {"",""}; // init with 2 size strning array

    int char_idx = 0;

    while(1){
        if(str[char_idx]==delimiter || char_idx==str.length()){
            break;
        }
        splitted_str[0] += str[char_idx];
        char_idx++;
    }

    char_idx++;
    
    while(1){
        if(str[char_idx]==delimiter || char_idx==str.length()){
            break;
        }
        splitted_str[1] += str[char_idx];
        char_idx++;
    }

    return splitted_str;

}

struct Configs* parserConfig(std::string content){

    struct Configs* config = new Configs();
    
    // extracting server ip

    std::smatch match;
    std::regex r_ip("\\d{1,3}.\\d{1,3}.\\d{1,3}.\\d{1,3}");
    std::regex_search(content,match,r_ip);
    if(match.length()==0) config->ip = "";

    else {
            std::string ip_ = match[0];
            config->ip = ip_;
        };

    // extracting port

    std::regex r_port("port=(\\d{1,4})");
    std::regex_search(content,match,r_port);
    if(match.length()==0) config->port = -1;
    else {
            std::string port_ = match[1];
            int port;
            if(sscanf(port_.c_str(),"%d",&port)!=1) config->port = -1;
            else config->port = port;
        };
    
    // extract routes
    
    std::vector<struct Route> routes;
    std::regex r_routes("/([a-zA-Z0-9]*),[a-zA-Z0-9/]*.html");
    std::sregex_iterator iter(content.begin(),content.end(),r_routes);
    std::sregex_iterator end;

    while(iter != end)
    {   
        struct Route route;
        std::vector<std::string> splitted_str = split((*iter)[0],',');
        route.route = splitted_str[0];
        route.route_file_path = splitted_str[1];
        routes.push_back(route);
        ++iter;
    }

    config->routes = routes;
    
    return config;
}

std::vector<std::string> parserCss(std::string html){


    std::vector<std::string> css_files;

    std::regex rgx("<link\\s+.*?href=\"(.*?)\"");
    std::smatch match;

    std::sregex_iterator iter(html.begin(),html.end(),rgx);
    std::sregex_iterator end;

    while(iter != end)
    {
        css_files.push_back((*iter)[1]);   
        ++iter;
    }

    return css_files;
}

std::vector<std::string> parserJs(std::string html){


    std::vector<std::string> js_files;

    std::regex rgx("<script\\s+.*?src=\"(.*?)\"");
    std::smatch match;

    std::sregex_iterator iter(html.begin(),html.end(),rgx);
    std::sregex_iterator end;

    while(iter != end)
    {
        js_files.push_back((*iter)[1]);   
        ++iter;
    }

    return js_files;
}

int bind_http_header(std::string* html,int status){


    std::string BASE_DIR = std::filesystem::current_path();
    std::string header_binded_html;
    if(status==500){
        if(file_reader(BASE_DIR + "/" + "responses/500.temp",&header_binded_html)){
            perror("Cannot find server responses\n.");
            exit(1);
        }
        if(file_reader(BASE_DIR + "/" + "responses/500.html",html)){
            perror("Cannot find server responses\n.");
            exit(1);
        }
    }

    else if(status==404){
        if(file_reader(BASE_DIR + "/" + "responses/404.temp",&header_binded_html)){
            perror("Cannot find server responses\n.");
            exit(1);
        }
        if(file_reader(BASE_DIR + "/" + "responses/404.html",html)){
            perror("Cannot find server responses\n.");
            exit(1);
        }
    }

    else if(status==200){
        if(file_reader(BASE_DIR + "/" + "responses/200.temp",&header_binded_html)){
            perror("Cannot find server responses\n.");
            exit(1);
        }
    }

    header_binded_html += "\n\n";
    *html = header_binded_html + *html;
    return 0;
}

int binderCss(std::string* html,std::vector<std::string> css_files,std::string HOST_DIR){

    /* 
        Status 1 = binded successfully
        status 0 = binding failed
    */

    std::string whole_css = "";
    std::string buffer;
    for(int i=0;i<css_files.size();i++){
        
        if(file_reader(HOST_DIR + "/" + css_files[i],&buffer)){
           return 1;
        }

        else{
            
            whole_css += buffer;
        }
        
    }



    *html = "<style>"+whole_css+"</style>"+*html;

    return 0;

}



int binderJs(std::string* html,std::vector<std::string> js_files,std::string HOST_DIR){

    /* 
        Status 1 = binded successfully
        status 0 = binding failed
    */

    std::string whole_js = "";
    std::string buffer;

    for(int i=0;i<js_files.size();i++){
        
        if(file_reader(HOST_DIR + "/" + js_files[i],&buffer)){
            return 1;
        }

        else{
            whole_js += buffer;
        }
        
    }



    *html = *html+"<script>"+whole_js+"</script>";

    return 0;

}


int html_preprocessor(std::string* html,std::string HOST_DIR,int status){

    /*
        Status Code:
            1 = server Err 500 | Css File not found
            2 = server Err 500 | Js File not found
            0 = server Success 200 | html Preprocessed successfully
    */

    if(status!=200) {
        bind_http_header(html,status);
        return 1;
    }

    std::vector<std::string> css_files =  parserCss(*html);
    std::vector<std::string> js_files =  parserJs(*html);
    

    if(binderCss(html,css_files,HOST_DIR)){
        std::cout << "display 500 file not found css"<<std::endl;
        return 1;
    }

    if(binderJs(html,js_files,HOST_DIR)){
        std::cout << "display 500 file not found js"<<std::endl;
        return 1; 
    }

    bind_http_header(html,status);

    return 0;

}

int process_request(char* request,std::string* route){
    
    *route = "";
    // Referer: http://192.168.0.4/
    // GET /about HTTP/1.1
    // std::regex route_regex("Referer: http://\\d{1,3}.\\d{1,3}.\\d{1,3}.\\d{1,3}/([a-zA-Z0-9/]*)");
    std::regex route_regex("GET /([a-zA-Z0-9/]*)");
    std::cmatch match;

    std::regex_search(request,match,route_regex);
    
    *route = match[1];


    std::cout << *route << std::endl;

    return 0;

}