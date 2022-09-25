#pragma once

#include <vector>


std::vector<std::string> parserCss(std::string html);
std::vector<std::string> parserJs(std::string html);
int binderCss(std::string* html,std::vector<std::string> css_files);
int binderJs(std::string* html,std::vector<std::string> js_files);
int html_preprocessor(std::string* html);