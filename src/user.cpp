#ifndef USER_CLASS_HPP
#define USER_CLASS_HPP

#include <string>
#include <iostream>
#include "constants/error_messages.hpp"
#include "constants/game_text.hpp"

static int num_players = 0;

class user {
    private:
        void defineMarker() {
            if (num_players == 1) {
                this->marker = 'O';
            } else if (num_players == 2) {
                this->marker = 'X';
            } else {
                throw error_msg::TOO_MANY_PLAYERS;
            }
        }
    protected:
        virtual ~user() = default;
    public:
        char marker;
        std::string name;
        user (std::string name) : name(name) {
            num_players++;
            defineMarker();
        }
        virtual void place_marker(Mills* m) {
        }
};

#endif