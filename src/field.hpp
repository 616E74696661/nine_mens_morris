#ifndef MILLS_CLASS_HPP
#define MILLS_CLASS_HPP

#include "constants/error_messages.hpp"
#include "position.hpp"
#include "user.hpp"
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <thread>

#ifdef _WIN32
const std::string OS = "windows";
#else
const std::string OS = "other"; // macos/linux
#endif

class Field {
public:
  Field() {
    current_phase = OPENING;
    stones_set = 0;
  }

  enum GamePhase {
    OPENING = 0,
    MIDGAME = 1,
    ENDGAME = 2
  };

  char get_field_marker_at_position(Position pos) {
    unsigned int y_pos = get_y_pos(pos.y);
    unsigned int x_pos = get_x_pos(pos.x);
    return field_template[y_pos][x_pos];
  }

  void field_output() {
    for (unsigned int i = 0; i < Y; i++) {
      if (i % 2 == 0 && i != Y - 1)
        std::cout << 7 - (i / 2) << "  ";
      else
        std::cout << "   ";
      std::cout << field_template[i] << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds((long long)ANIMATION_TIME));
    }
  }

  bool valid_coordinates(unsigned int x, unsigned int y) {
    return (x < 8 && y < 8);
  }

  void clear_console() {
    for (unsigned int i = 0; i < 30; i++) {
      std::cout << "\n";
      std::this_thread::sleep_for(std::chrono::milliseconds((long long)ANIMATION_TIME));
    }
    if (OS == "windows")
      system("CLS");
    else // for linux and macos
      system("clear");
  }

  void stone_set() {
    stones_set++;
  }

  bool player_move_stone(char active_player_marker, Position& old_pos, Position& new_pos) {
    if (!valid_coordinates(old_pos.x, old_pos.y))
      throw std::out_of_range(error_msg::INVALID_POSITION);
    if (!valid_coordinates(new_pos.x, new_pos.y))
      throw std::out_of_range(error_msg::INVALID_POSITION);
    unsigned int old_y_pos = get_y_pos(old_pos.y);
    unsigned int old_x_pos = get_x_pos(old_pos.x);
    unsigned int new_y_pos = get_y_pos(new_pos.y);
    unsigned int new_x_pos = get_x_pos(new_pos.x);

    // clear_console();

    // Check if new position is empty
    if (get_field_marker_at_position(new_pos) != '#') {
      throw std::invalid_argument(error_msg::POSITION_OCCUPIED);
    }
    std::cout << "New position valid" << std::endl;

    if (get_field_marker_at_position(old_pos) == active_player_marker) {

      std::cout << "Old position valid" << std::endl;

      field_template[old_y_pos][old_x_pos] = '#';
      field_template[new_y_pos][new_x_pos] = active_player_marker;
      std::cout << "successfully moved :3" << std::endl;
      return true;
    } else {
      throw std::invalid_argument(error_msg::INVALID_MOVE);
    }
  }

  bool player_remove_stone(User& active_player, Position& pos) {
    if (!valid_coordinates(pos.x, pos.y))
      throw std::out_of_range(error_msg::INVALID_POSITION);
    unsigned int y_pos = get_y_pos(pos.y);
    unsigned int x_pos = get_x_pos(pos.x);

    // clear_console();

    if (get_field_marker_at_position(pos) == active_player.marker) {
      field_template[y_pos][x_pos] = '#';
      std::cout << "successfully moved :3" << std::endl;
      return true;
    } else {
      throw std::invalid_argument(error_msg::INVALID_MOVE);
    }
  }

  bool opponent_remove_stone(char active_player_marker, Position& pos, User& other_player) {
    if (!valid_coordinates(pos.x, pos.y))
      throw std::out_of_range(error_msg::INVALID_POSITION);

    unsigned int pos_y = get_y_pos(pos.y);
    unsigned int pos_x = get_x_pos(pos.x);

    // clear_console();

    if (get_field_marker_at_position(pos) != active_player_marker && get_field_marker_at_position(pos) != '#') {
      field_template[pos_y][pos_x] = '#';
      std::cout << "successfully removed :3" << std::endl;
      // noch stones-- bei anderen user hinzufuegen
      other_player.remove_stone();
      return true;
    } else {
      throw std::invalid_argument(error_msg::INVALID_REMOVAL);
    }
  }

  GamePhase get_current_phase() {
    return current_phase;
  }

  void next_phase() {
    if (current_phase < ENDGAME)
      current_phase = static_cast<GamePhase>(static_cast<unsigned int>(current_phase) + 1);
    std::cout << "NEXT GAMEPHASE" << std::endl;
  }

  bool player_set_stone(User& active_player, Position& pos) {
    if (!valid_coordinates(pos.x, pos.y))
      throw std::out_of_range(error_msg::INVALID_POSITION);

    unsigned int y_pos = get_y_pos(pos.y);
    unsigned int x_pos = get_x_pos(pos.x);
    // clear_console();

    if (get_field_marker_at_position(pos) != T) {
      throw std::invalid_argument(error_msg::POSITION_OCCUPIED);
    }

    field_template[y_pos][x_pos] = active_player.marker;
    active_player.set_stone();
    std::cout << "successfully set :3" << std::endl;
    // stones_set++;
    return true;
  }

  bool check_mill(std::array<Position, 3> mill, char marker) {

    unsigned int counter = 0;

    for (const auto& pos : mill) {
      if (get_field_marker_at_position(pos) == marker) {
        counter++;
      }
    }
    return counter == 3;
  }

private:
  static const unsigned int ANIMATION_TIME = 10;
  static const unsigned int X = 26;
  static const short Y = 15;
  static const short STONES = 18;
  static const char T = '#';
  GamePhase current_phase;
  unsigned int stones_set;

  char field_template[Y][X] = {
      "#-----------#-----------#",
      "|           |           |",
      "|   #-------#-------#   |",
      "|   |       |       |   |",
      "|   |   #---#---#   |   |",
      "|   |   |       |   |   |",
      "#---#---#       #---#---#",
      "|   |   |       |   |   |",
      "|   |   #---#---#   |   |",
      "|   |       |       |   |",
      "|   #-------#-------#   |",
      "|           |           |",
      "#-----------#-----------#",
      "",
      "1   2   3   4   5   6   7"};

  unsigned int get_y_pos(unsigned int y_pos) { return ((7 - (y_pos)) * 2); }

  // irrelevant when saving as [7][7] arr
  unsigned int get_x_pos(unsigned int x_pos) { return ((x_pos - 1) * 4); }
};
#endif