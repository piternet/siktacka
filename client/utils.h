//
// Created by piternet on 17.08.17.
//

#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H

#include <iostream>
#include <algorithm>
#include <sstream>
#include <chrono>

using std::string;

const char NEWLINE = '\n';
const int SEND_INTERVAL = 20;
const int POLL_INTERVAL = 2;
const int BUFF_SIZE = 1024;
const int RECV_BUFF_SIZE = 512;
const int MIN_BUFF_SIZE = 14; // 8 + 1 + 4 + 1 ('\0')
const int EVENT_NEW_GAME_SIZE = 13;

const int PLAYER_NAME_BOTTOM_CHAR = 33;
const int PLAYER_NAME_UPPER_CHAR = 126;

typedef std::chrono::time_point<std::chrono::system_clock> Time;

void die(string msg);
void printError(string msg);
string intToString(int number);
uint64_t getTime();

#endif //CLIENT_UTILS_H
