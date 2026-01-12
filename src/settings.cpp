#ifndef SETTINGS_CLASS_HPP
#define SETTINGS_CLASS_HPP

#include "bot_user.cpp"
#include "constants/error_messages.hpp"
#include "constants/game_text.hpp"
#include "player_user.cpp"
#include <exception>
#include <vector>

enum GameMode {
  PLAYER_VS_PLAYER = 1,
  PLAYER_VS_BOT = 2
};

class Settings {
public:
  unsigned int mode;
  std::vector<User*> players;

  std::vector<User*> setup() {
    std::string output;
    while (true) {
      try {
        mode = Helper::read_uint(game_text::GAME_MODE);

        if (mode == PLAYER_VS_PLAYER) {
          output = "Player vs Player mode selected.";
          players.push_back(new PlayerUser("Player 1"));
          players.push_back(new PlayerUser("Player 2"));
          break;

        } else if (mode == PLAYER_VS_BOT) {
          output = "Player vs Bot mode selected.";
          players.push_back(new PlayerUser("Player 1"));
          players.push_back(new BotUser("Bot"));
          break;
        } else {

          std::cout << error_msg::INVALID_SELECTION << std::endl;
          std::cout << "--------------------------" << std::endl;
        }
      } catch (std::exception) {
        std::cout << error_msg::INVALID_SELECTION << std::endl;
        std::cout << "--------------------------" << std::endl;
      }
    }
    Helper::clear_console();
    std::cout << output << std::endl;
    return players;
  }
};

#endif