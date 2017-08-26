//
// Created by piternet on 22.08.17.
//

#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H

#include <cstdint>
#include <cstring>
#include "utils.h"
#include <netinet/in.h>

class Game {
public:
    Game () {}
    std::vector<string> players;
    uint32_t maxx, maxy;

    void setPlayers(std::vector<string> &_players);
};

#endif //CLIENT_GAME_H
