#ifndef MILLS_CLASS_HPP
#define MILLS_CLASS_HPP

#include "constants/error_messages.hpp"
#include "helper.hpp"
#include "position.hpp"
#include "user.hpp"
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <thread>

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

  char get_field_marker_at_position(Position& pos) {
    if (!valid_coordinates(pos.x, pos.y)) {
      throw std::out_of_range(error_msg::INVALID_POSITION);
    }
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
      std::this_thread::sleep_for(std::chrono::milliseconds((long long)Helper::ANIMATION_TIME));
    }
  }

  bool valid_coordinates(unsigned int x, unsigned int y) const {
    return (x > 0 && x < 8 && y < 8 && y > 0);
  }

  void stone_set() {
    stones_set++;
  }

  // bool player_move_stone(User& active_player, Position& old_pos, Position& new_pos, bool three_stones_left = false) {
  //   // Validate coordinates first
  //   if (!valid_coordinates(old_pos.x, old_pos.y))
  //     throw std::out_of_range(error_msg::INVALID_POSITION);
  //   if (!valid_coordinates(new_pos.x, new_pos.y))
  //     throw std::out_of_range(error_msg::INVALID_POSITION);

  //   // Check if old position has player's stone
  //   if (get_field_marker_at_position(old_pos) != active_player.marker) {
  //     throw std::invalid_argument(error_msg::INVALID_MOVE);
  //   }
  //   if (!three_stones_left && !Position::is_neighbour(old_pos, new_pos))
  //     throw std::invalid_argument(error_msg::NOT_NEIGHBOUR);
  //   // Check if new position is empty
  //   if (get_field_marker_at_position(new_pos) != T) {
  //     throw std::invalid_argument(error_msg::POSITION_OCCUPIED);
  //   }

  //   // Perform the move
  //   unsigned int old_y_pos = get_y_pos(old_pos.y);
  //   unsigned int old_x_pos = get_x_pos(old_pos.x);
  //   unsigned int new_y_pos = get_y_pos(new_pos.y);
  //   unsigned int new_x_pos = get_x_pos(new_pos.x);

  //   field_template[old_y_pos][old_x_pos] = T;
  //   field_template[new_y_pos][new_x_pos] = active_player.marker;

  //   return true;
  // }

  bool player_remove_stone(User& active_player, Position& pos) {
    // Validate coordinates
    if (!valid_coordinates(pos.x, pos.y))
      throw std::out_of_range(error_msg::INVALID_POSITION);

    // Check if position has player's stone
    if (get_field_marker_at_position(pos) != active_player.marker) {
      throw std::invalid_argument(error_msg::INVALID_MOVE);
    }

    // Remove the stone
    unsigned int y_pos = get_y_pos(pos.y);
    unsigned int x_pos = get_x_pos(pos.x);
    field_template[y_pos][x_pos] = T;

    return true;
  }

  bool opponent_remove_stone(char active_player_marker, Position& pos, User& other_player) {
    // Validate coordinates
    if (!valid_coordinates(pos.x, pos.y))
      throw std::out_of_range(error_msg::INVALID_POSITION);

    // Check if position has opponent's stone (not active player's, not empty)
    char marker = get_field_marker_at_position(pos);
    if (marker == active_player_marker || marker == T) {
      throw std::invalid_argument(error_msg::INVALID_REMOVAL);
    }

    // Remove the stone
    unsigned int pos_y = get_y_pos(pos.y);
    unsigned int pos_x = get_x_pos(pos.x);
    field_template[pos_y][pos_x] = T;
    other_player.remove_stone();

    return true;
  }

  GamePhase get_current_phase() const {
    return current_phase;
  }

  void next_phase() {
    if (current_phase < ENDGAME)
      current_phase = static_cast<GamePhase>(static_cast<unsigned int>(current_phase) + 1);
  }

  bool player_set_stone(User& active_player, Position& pos, bool moved = false) {
    // Validate coordinates
    if (!valid_coordinates(pos.x, pos.y))
      throw std::out_of_range(error_msg::INVALID_POSITION);

    // Check if position is empty
    if (get_field_marker_at_position(pos) != T) {
      throw std::invalid_argument(error_msg::POSITION_OCCUPIED);
    }

    // Place the stone
    unsigned int y_pos = get_y_pos(pos.y);
    unsigned int x_pos = get_x_pos(pos.x);
    field_template[y_pos][x_pos] = active_player.marker;

    if (!moved) {
      active_player.set_stone();
    }

    return true;
  }

  bool check_mill(std::array<Position, 3> mill, char marker) {
    unsigned int counter = 0;

    for (auto pos : mill) {
      try {
        if (get_field_marker_at_position(pos) == marker) {
          counter++;
        }
      } catch (const std::exception&) {
        // Invalid position in mill array, skip it
        return false;
      }
    }
    return counter == 3;
  }

private:
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

  unsigned int get_y_pos(unsigned int y_pos) const {
    return ((7 - (y_pos)) * 2);
  }

  unsigned int get_x_pos(unsigned int x_pos) const {
    return ((x_pos - 1) * 4);
  }
};
#endif