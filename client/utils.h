//
// Created by piternet on 17.08.17.
//

#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H

#include <iostream>
#include <algorithm>
#include <sstream>

using std::string;

const char NEWLINE = '\n';
const int BUFF_SIZE = 1024;

void die(string msg);
string intToString(int number);

#endif //CLIENT_UTILS_H
