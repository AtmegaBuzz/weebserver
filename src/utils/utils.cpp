#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>

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


int binderCss(std::string* html,std::vector<std::string> css_files){

    /* 
        Status 1 = binded successfully
        status 0 = binding failed
    */

    std::stringstream buffer;
    for(int i=0;i<css_files.size();i++){
        
        std::ifstream cssfile_fd;
        cssfile_fd.open(css_files[i]);
        if(!cssfile_fd){
            return 0;
        }

        buffer << cssfile_fd.rdbuf;
        *html << buffer.str();
        cssfile_fd.close();
    }


    return 1;

}