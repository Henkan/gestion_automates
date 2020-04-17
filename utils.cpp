//
// Created by Sylvain on 05/04/2020.
//

#include "utils.h"

std::vector<std::string> splitString(std::string str, char delimiter) {
    std::vector<std::string> rep;
    std::string word("");
    for (std::string::iterator str_it = str.begin(); str_it != str.end(); ++str_it) {
        if (*str_it != delimiter) {
            word += *str_it;
        } else if (word != "") {
            rep.push_back(word);
            word = "";
        }
    }
    if (word != "") rep.push_back(word);
    return rep;
}