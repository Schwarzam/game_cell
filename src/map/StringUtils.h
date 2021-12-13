//
// Created by gusta on 13/12/2021.
//

#ifndef THECELL_STRINGUTILS_H
#define THECELL_STRINGUTILS_H
#include <iostream>
#include "vector"

inline const std::string WHITESPACE = " \n\r\t\f\v";
std::map<std::string, float> Map::metadata;

std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

bool is_number(std::string s)
{
    s = trim(s);
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

bool contains(const std::string& s1, const std::string& s2){
    return (s1.find(s2) != std::string::npos);
}

void split(std::string s, const std::string& delimiter, std::vector<std::string> *v){
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        v->emplace_back(token);
        s.erase(0, pos + delimiter.length());
    }
    v->emplace_back(s);
}

#endif //THECELL_STRINGUTILS_H
