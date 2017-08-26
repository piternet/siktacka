//
// Created by piternet on 25.08.17.
//


#include "event.h"

string newLine() {
    return "\n";
}

string Event::toGuiRepr(Game &game) {
    string repr = "";
   // std::cout << "wysylam do gui cos o typie " << (int)this->type << std::endl;
    switch(this->type) {
        case NEW_GAME: {
            repr += "NEW_GAME ";
            repr += std::to_string(this->data->newGame.maxx);
            repr += " ";
            repr += std::to_string(this->data->newGame.maxy);
            for(const auto &name : this->data->newGame.playerNames)
                repr += " " + name;
            break;
        }
        case PIXEL: {
            repr += "PIXEL ";
            repr += std::to_string(this->data->pixel.x);
            repr += " ";
            repr += std::to_string(this->data->pixel.y);
            repr += " ";
            repr += game.players[static_cast<int>(this->data->pixel.playerNumber)];
            //std::cout << repr << std::endl;
            break;
        }
        case PLAYER_ELIMINATED: {
            repr += "PLAYER_ELIMINATED ";
            repr += game.players[static_cast<int>(this->data->playerEliminated.playerNumber)];
            break;
        }
        default:
            return "X";
    }
    return repr + newLine();
}

Event Event::makeEvent(char *receiveBuffer, int &pos, Game &game) {
    Event event;
    uint32_t len, event_no;
    uint8_t event_type;
    uint32_t crc32;

    memcpy(&len, receiveBuffer+pos, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    memcpy(&event_no, receiveBuffer+pos, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    memcpy(&event_type, receiveBuffer+pos, sizeof(uint8_t));
    pos += sizeof(uint8_t);
    len = ntohl(len);
    if(len == 0 || len > 1000)
        return Event(0);
    event_no = ntohl(event_no);
    std::cout << "otrzymalem:"
              << ", len: " << len
              << ", event_no: " << event_no
              << ", event_type" << (int)event_type <<  std::endl;
    event.data = std::make_shared<Data>(Data());
    switch(event_type) {
        case NEW_GAME: {
            uint32_t maxx, maxy;
            memcpy(&maxx, receiveBuffer+pos, sizeof(uint32_t));
            pos += sizeof(uint32_t);
            memcpy(&maxy, receiveBuffer+pos, sizeof(uint32_t));
            pos += sizeof(uint32_t);
            maxx = ntohl(maxx);
            maxy = ntohl(maxy);
            string name;
            std::vector<string> names;
            for(int i=0; i<len-EVENT_NEW_GAME_SIZE; i++) {
                if(receiveBuffer[i+pos] == '\0') {
                    std::cout << name << ", ";
                    names.push_back(name);
                    name = "";
                }
                else
                    name += receiveBuffer[i+21];
            }
            pos += len-EVENT_NEW_GAME_SIZE;
            event.data->newGame = NewGameStruct(maxx, maxy, names);

            game.setPlayers(names);
            game.maxx = maxx;
            game.maxy = maxy;
            break;
        }
        case PIXEL: {
            uint8_t player_number;
            uint32_t x, y;
            memcpy(&player_number, receiveBuffer+pos, sizeof(uint8_t));
            pos += sizeof(uint8_t);
            memcpy(&x, receiveBuffer+pos, sizeof(uint32_t));
            pos += sizeof(uint32_t);
            memcpy(&y, receiveBuffer+pos, sizeof(uint32_t));
            pos += sizeof(uint32_t);
            x = ntohl(x);
            y = ntohl(y);
            std::cout << " x = " << x << ", y = " << y << ", player num = " << (int)player_number << std::endl;
            event.data->pixel = PixelStruct(player_number, x, y);
            break;
        }
        case PLAYER_ELIMINATED: {
            break;
        }
        case GAME_OVER: {
            break;
        }
    }
    event.len = len;
    event.number = event_no;
    event.type = static_cast<EventType>(event_type);
    event.crc32 = 0; //todo
    pos += sizeof(uint32_t);
    return event;
}