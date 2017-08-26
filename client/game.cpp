//
// Created by piternet on 22.08.17.
//


#include <cstdlib>
#include "game.h"

void Game::setPlayers(std::vector<string> &_players) {
    // sort them and then set
    sort(_players.begin(), _players.end());
    this->players = _players;
}