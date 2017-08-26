//
// Created by piternet on 17.08.17.
//

#include "utils.h"

void die(string msg) {
    printError(msg);
    exit(1);
}

void printError(string msg) {
    std::cout << msg << std::endl;
}

string intToString(int number) {
    std::ostringstream ss;
    ss << number;
    return ss.str();
}

uint64_t getTime() {
    Time now = std::chrono::system_clock::now(), base = std::chrono::system_clock::from_time_t(0);
    auto ticks = std::chrono::duration_cast<std::chrono::microseconds>(now - base);
    return static_cast<uint64_t>(ticks.count());
}
