//
// Created by piternet on 17.08.17.
//

#include "utils.h"

void die(string msg) {
    std::cout << msg << std::endl;
    exit(1);
}

string intToString(int number) {
    std::ostringstream ss;
    ss << number;
    return ss.str();
}
