//
// Created by piternet on 17.08.17.
//

#include "parser.h"

ClientInfo clientInfo;

void parseServerInfo(string serverHost) {
    std::size_t found = serverHost.find_last_of(":");
    // we assume that IPv6 addresses won't be given with port numbers
    if(std::count(serverHost.begin(), serverHost.end(), ':') <= 1 && found != string::npos) {
        string port = serverHost.substr(found+1), host = serverHost.substr(0, found);
        clientInfo.serverHost = host;
        clientInfo.serverPort = std::stoi(port);
    }
    else {
        clientInfo.serverHost = serverHost;
        clientInfo.serverPort = DEFAULT_SERVER_PORT;
    }
}

void parseUIInfo(string uiHost) {
    std::size_t found = uiHost.find_last_of(":");
    // we assume that IPv6 addresses won't be given with port numbers
    if(std::count(uiHost.begin(), uiHost.end(), ':') <= 1 && found != string::npos) {
        string port = uiHost.substr(found+1), host = uiHost.substr(0, found);
        clientInfo.uiHost = host;
        clientInfo.uiPort = std::stoi(port);
    }
    else {
        clientInfo.uiHost = uiHost;
        clientInfo.uiPort = DEFAULT_UI_PORT;
    }
}

ClientInfo parseArguments(int argc, char **argv) {
    if(argc < MIN_ARGS)
        die(INIT_MSG);
    clientInfo.playerName = argv[1];
    parseServerInfo(string(argv[2]));

    if(argc > MIN_ARGS)
        parseUIInfo(string(argv[3]));
    else {
        clientInfo.uiHost = DEFAULT_UI_HOST;
        clientInfo.uiPort = DEFAULT_UI_PORT;
    }
    return clientInfo;
}
