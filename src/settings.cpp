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

  /**
   * @brief Initializes two player's based on the user's input. Either Player vs Player or Player vs Bot
   *
   * @return std::vector<User*> Returns two users
   */
  std::vector<User*> setup(Field& field, int& iteration) {
    std::string output;
    while (true) {
      try {

        mode = Helper::read_uint(game_text::GAME_MODE);

        if (mode == 9) {
          DataIO data_io;
          int mode_data;
          std::vector<int> stones_data;
          if (data_io.import_data(field, mode_data, stones_data, iteration)) {
            output = "Loading game...\n";
            switch (mode_data) {
            case PLAYER_VS_PLAYER:
              output += game_text::PVP_SELECTED;
              players.push_back(new PlayerUser("Player 1", stones_data[0], stones_data[1]));
              players.push_back(new PlayerUser("Player 2", stones_data[2], stones_data[3]));
              break;
            case PLAYER_VS_BOT:
              output += game_text::PVBOT_SELECTED;
              players.push_back(new PlayerUser("Player 1", stones_data[0], stones_data[1]));
              players.push_back(new BotUser("Bot", stones_data[2], stones_data[3]));
              break;
            }
            break;
          }
          Helper::clear_console();
          std::cout << "No Gamestate found." << std::endl;
          mode = Helper::read_uint(game_text::GAME_MODE_NO_FILE);
        }
        if (mode == PLAYER_VS_PLAYER) {
          output = game_text::PVP_SELECTED;
          players.push_back(new PlayerUser("Player 1"));
          players.push_back(new PlayerUser("Player 2"));
          break;

        } else if (mode == PLAYER_VS_BOT) {
          output = game_text::PVBOT_SELECTED;
          players.push_back(new PlayerUser("Player 1"));
          players.push_back(new BotUser("Bot"));
          break;
        } else
          break;

        std::cout << error_msg::INVALID_SELECTION << std::endl;
        std::cout << "--------------------------" << std::endl;
      } catch (const Helper::close_game&) {
        throw;
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