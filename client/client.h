//
// Created by piternet on 17.08.17.
//

#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <zconf.h>
#include <mutex>
#include "utils.h"
#include "parser.h"

struct GuiInfo {
    bool leftPressed, rightPressed;
    int socket;
    GuiInfo(bool _leftPressed, bool _rightPressed) : leftPressed(_leftPressed), rightPressed(_rightPressed) {

    }
};

std::mutex guiLock;

namespace turns {
    const string LEFT_DOWN = "LEFT_KEY_DOWN";
    const string LEFT_UP = "LEFT_KEY_UP";
    const string RIGHT_DOWN = "RIGHT_KEY_DOWN";
    const string RIGHT_UP = "RIGHT_KEY_UP";
}

#endif //CLIENT_CLIENT_H
