#ifndef SETTINGS_CLASS_HPP
#define SETTINGS_CLASS_HPP

#include "constants/error_messages.hpp"
#include "constants/game_text.hpp"
#include "player_user.cpp"
#include "bot_user.cpp"
#include "user.cpp"
#include <iostream>
#include <vector>

enum GameMode {
    PLAYER_VS_PLAYER = 1,
    PLAYER_VS_BOT = 2
};

class Settings {
public:
    std::vector<user*> setup() {
        std::cout << game_text::GAME_MODE << std::endl;
        int mode;
        std::cin >> mode;
        if (mode == PLAYER_VS_PLAYER) {
            std::cout << "Player vs Player mode selected." << std::endl;

            std::vector<user*> players;
            players.push_back(new player_user("Player 1")); 
            players.push_back(new player_user("Player 2"));
            return players;

        } else if (mode == PLAYER_VS_BOT) {
            std::cout << "Player vs Bot mode selected." << std::endl;

            std::vector<user*> players;
            players.push_back(new player_user("Player 1"));
            players.push_back(new bot_user("Bot"));            
            return players;

        } else {
            std::cout << error_msg::INVALID_SELECTION << std::endl;
            std::cout<< "--------------------------" << std::endl;
            return setup();
        }        
    }
};


#endif