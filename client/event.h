//
// Created by piternet on 25.08.17.
//

#ifndef CLIENT_EVENT_H
#define CLIENT_EVENT_H


#include <cstdint>
#include <memory>
#include <cstring>
#include <netinet/in.h>
#include "utils.h"
#include "game.h"

enum EventType : uint8_t {
    NEW_GAME            = 0,
    PIXEL               = 1,
    PLAYER_ELIMINATED   = 2,
    GAME_OVER           = 3
};




class NewGameStruct {
public:
    uint32_t maxx, maxy;
    std::vector<string> playerNames;

    NewGameStruct() {}
    NewGameStruct(uint32_t _maxx, uint32_t _maxy) : maxx(_maxx), maxy(_maxy) {}
    NewGameStruct(uint32_t _maxx, uint32_t _maxy, const std::vector<string> &_playerNames)
            : maxx(_maxx), maxy(_maxy) {
        playerNames.reserve(_playerNames.size());
        for(const auto &name : _playerNames)
            playerNames.push_back(name);
    }

};

class PixelStruct {
public:
    uint8_t playerNumber;
    uint32_t x, y;

    PixelStruct() {}
    PixelStruct(uint8_t _playerNumber, uint32_t _x, uint32_t _y)
            : playerNumber(_playerNumber), x(_x), y(_y) {
    }
};

class PlayerEliminatedStruct {
public:
    uint8_t playerNumber;

    PlayerEliminatedStruct() {}
    PlayerEliminatedStruct(uint8_t _playerNumber) : playerNumber(_playerNumber) {

    }
};

class Data {
public:
    NewGameStruct newGame;
    PixelStruct pixel;
    PlayerEliminatedStruct playerEliminated;

    Data() {}
};

class Event {

public:
    uint32_t len, number, crc32;
    EventType type;
    std::shared_ptr<Data> data;
    Event() {}
    Event(uint32_t _len) : len(_len) {}
    string toGuiRepr(Game &game);
    static Event makeEvent(char *buffer, int &pos, Game &game);
};


#endif //CLIENT_EVENT_H
