#ifndef SETTINGS_CLASS_HPP
#define SETTINGS_CLASS_HPP

#include "bot_user.cpp"
#include "constants/error_messages.hpp"
#include "constants/game_text.hpp"
#include "data_io.hpp"
#include "helper.hpp"
#include "player_user.cpp"
#include <exception>
#include <iostream>
#include <vector>

enum GameMode {
  PLAYER_VS_PLAYER = 1,
  PLAYER_VS_BOT = 2
};

class Settings {
public:
  int mode;
  std::vector<User*> players;

  std::vector<User*> setup(Field& field) {
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
        } else if (mode == 9) {
          DataIO data_io;

          int mode_data;
          std::vector<int> stone_data;
          int active_user;

          data_io.import_data(field, mode_data, stone_data, active_user);

          std::cout << "mode_data: " << mode_data << std::endl;

          std::cout << "stone_data: " << std::endl;
          for (auto stone_ : stone_data) {
            std::cout << "\t" << stone_ << std::endl;
          }

          std::cout << "active_user: " << active_user << std::endl;

          switch (mode_data) {
          case PLAYER_VS_PLAYER:
            players.push_back(new PlayerUser("Player 1", stone_data[0], stone_data[1]));
            players.push_back(new PlayerUser("Player 2", stone_data[2], stone_data[3]));
            break;
          case PLAYER_VS_BOT:
            players.push_back(new PlayerUser("Player 1", stone_data[0], stone_data[1]));
            players.push_back(new BotUser("Bot", stone_data[2], stone_data[3]));
            break;
          }
        } else {

          std::cout << error_msg::INVALID_SELECTION << std::endl;
          std::cout << "--------------------------" << std::endl;
        }
      } catch (std::exception) {
        std::cout << error_msg::INVALID_SELECTION << std::endl;
        std::cout << "----------------------daaaaa----" << std::endl;
      }
    }
    Helper::clear_console();
    std::cout << output << std::endl;
    return players;
  }
};

#endif