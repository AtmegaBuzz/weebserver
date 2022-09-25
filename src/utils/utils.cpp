#include <filesystem>
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

    std::string whole_css = "";

    for(int i=0;i<css_files.size();i++){
        
        std::filesystem::path cwd = std::filesystem::current_path();

        std::ifstream cssfile_fd(cwd / css_files[i]);

        if(cssfile_fd.is_open()){

            std::string chunk;

            while(cssfile_fd>>chunk){
                whole_css += chunk;
            }

            cssfile_fd.close();
        }
        else{
            return 1;
        }
        
    }



    *html = "<style>"+whole_css+"</style>"+*html;

    return 0;

}



int binderJs(std::string* html,std::vector<std::string> js_files){

    /* 
        Status 1 = binded successfully
        status 0 = binding failed
    */

    std::string whole_js = "";

    for(int i=0;i<js_files.size();i++){
        
        std::filesystem::path cwd = std::filesystem::current_path();

        std::ifstream jsfile_fd(cwd / js_files[i]);

        if(jsfile_fd.is_open()){

            std::string chunk;

            while(jsfile_fd>>chunk){
                whole_js += chunk;
            }

            jsfile_fd.close();
        }
        else{
            return 1;
        }
        
    }



    *html = *html+"<script>"+whole_js+"</script>";

    return 0;

}


int html_preprocessor(std::string* html){

    /*
        Status Code:
            1 = server Err 500 | Css File not found
            2 = server Err 500 | Js File not found
            0 = server Success 200 | html Preprocessed successfully
    */

    std::vector<std::string> css_files =  parserCss(*html);
    std::vector<std::string> js_files =  parserJs(*html);
    

    if(binderCss(html,css_files)){
        std::cout << "display 500 file not found css"<<std::endl;
        return 1;
    }

    if(binderJs(html,js_files)){
        std::cout << "display 500 file not found js"<<std::endl;
        return 2; 
    }

    return 0;

}