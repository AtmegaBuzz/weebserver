#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <string.h>
#include <stdlib.h>



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


struct configs* parserConfig(std::string content){

    struct Configs* config = (struct Configs*) malloc(sizeof(struct Config));
    
    
    // extracting server ip

    std::smatch match;
    std::cout << content << std::endl;
    std::regex r_ip("\\d{1,3}.\\d{1,3}.\\d{1,3}.\\d{1,3}");
    std::regex_search(content,match,r_ip);
    if(match.length()==0) config->ip = NULL;
    else config->ip = match[0];

    // extracting port

    std::regex r_port("port=(\\d{1,4})");
    std::regex_search(content,match,r_port);
    if(match.length()<2) config->port = NULL;
    else config->port = match[1];
    
    // extract routes
    std::regex r_routes("routes:{(*)}?;");
    std::regex_search(content,match,r_routes);
    std::cout << match[0] << std::endl; 
    // if(match.length()<2) CONFIGS.port = NULL;
    // else CONFIGS.port = match[1];


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


int html_preprocessor(std::string* html,std::string HOST_DIR){

    /*
        Status Code:
            1 = server Err 500 | Css File not found
            2 = server Err 500 | Js File not found
            0 = server Success 200 | html Preprocessed successfully
    */

    std::vector<std::string> css_files =  parserCss(*html);
    std::vector<std::string> js_files =  parserJs(*html);
    

    if(binderCss(html,css_files,HOST_DIR)){
        std::cout << "display 500 file not found css"<<std::endl;
        return 1;
    }

    if(binderJs(html,js_files,HOST_DIR)){
        std::cout << "display 500 file not found js"<<std::endl;
        return 2; 
    }

    return 0;

}