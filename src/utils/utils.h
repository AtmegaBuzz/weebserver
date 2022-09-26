#pragma once

#include <vector>


std::vector<std::string> parserCss(std::string html);
std::vector<std::string> parserJs(std::string html);
int binderCss(std::string* html,std::vector<std::string> css_files,std::string HOST_DIR);
int binderJs(std::string* html,std::vector<std::string> js_files,std::string HOST_DIR);
int html_preprocessor(std::string* html,std::string HOST_DIR);
int file_reader(std::string file_path,std::string *buffer);