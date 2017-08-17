//
// Created by piternet on 17.08.17.
//

#ifndef KLIENT_CLIENT_H
#define KLIENT_CLIENT_H

#include <string>

using std::string;

const string INIT_MSG = "Usage: ./siktacka-client player_name game_server_host[:port] [ui_server_host[:port]]";
const int MIN_ARGS = 3;

const int DEFAULT_SERVER_PORT = 12345;
const int DEFAULT_UI_PORT = 12346;
const string DEFAULT_UI_HOST = "localhost";

void die(string msg) {
    std::cout << msg << std::endl;
    exit(1);
}

struct ClientInfo {
    string playerName, serverHost, uiHost;
    int serverPort, uiPort;
};

ClientInfo clientInfo;

#endif //KLIENT_CLIENT_H
