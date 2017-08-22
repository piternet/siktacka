//
// Created by piternet on 17.08.17.
//

#ifndef CLIENT_PARSER_H
#define CLIENT_PARSER_H

#include "utils.h"

const string INIT_MSG = "Usage: ./siktacka-client player_name game_server_host[:port] [ui_server_host[:port]]";
const int MIN_ARGS = 3;

const int DEFAULT_SERVER_PORT = 12345;
const int DEFAULT_UI_PORT = 12346;
const string DEFAULT_UI_HOST = "localhost";

struct ClientInfo {
    string playerName, serverHost, uiHost;
    int serverPort, uiPort;
};

ClientInfo parseArguments(int argc, char **argv);

#endif //CLIENT_PARSER_H
