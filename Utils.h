//
// Created by User on 04/08/2022.
//

#ifndef C_VERSION_UTILS_H
#define C_VERSION_UTILS_H

#include "includes.h"

class Utils {
public:
    static std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }
};


#endif //C_VERSION_UTILS_H
