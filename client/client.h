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
#include <thread>
#include <queue>
#include "utils.h"
#include "parser.h"
#include "game.h"
#include "event.h"
#include <poll.h>

struct GuiInfo {
    bool leftPressed, rightPressed;
    int socket;
    struct pollfd ufds;
    GuiInfo(bool _leftPressed, bool _rightPressed) : leftPressed(_leftPressed), rightPressed(_rightPressed) {

    }
};

struct ServerInfo {
    int socket;
    struct sockaddr_storage server;
    socklen_t len;
    struct pollfd ufds;
};

struct ClientData {
    int8_t turn_direction = 0;
    uint32_t next_expected_event_no = 0;
    uint64_t sessionId;
    string playerName;
    ClientData() {}
    ClientData(string _playerName) : playerName(_playerName) {
        turn_direction = 0;
        next_expected_event_no = 0;
    }
};

// global variables
ServerInfo serverInfo;
ClientData clientData;
Game game;
std::mutex dataLock;
bool running = true;
std::queue<Event> eventsToGui;

namespace turns {
    const string LEFT_DOWN = "LEFT_KEY_DOWN";
    const string LEFT_UP = "LEFT_KEY_UP";
    const string RIGHT_DOWN = "RIGHT_KEY_DOWN";
    const string RIGHT_UP = "RIGHT_KEY_UP";
}

#endif //CLIENT_CLIENT_H
