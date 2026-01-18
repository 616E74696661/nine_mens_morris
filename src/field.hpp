#ifndef MILLS_CLASS_HPP
#define MILLS_CLASS_HPP

#include "constants/error_messages.hpp"
#include "helper.hpp"
#include "position.hpp"
#include "user.hpp"
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

class Field {
public:
  Field(std::vector<std::string> field_string = {}) {
    current_phase = OPENING;
    stones_set = 0;
    if (field_string.empty())
      field_template = field_string;
  }

  enum GamePhase {
    OPENING = 0,
    MIDGAME = 1,
    ENDGAME = 2
  };
  static const char EMPTY_FIELD = '#';

  char get_field_marker_at_position(Position& pos) {
    if (!pos.is_valid()) {
      throw std::out_of_range(error_msg::INVALID_POSITION);
    }

    int y_pos = get_y_pos(pos.y);
    int x_pos = get_x_pos(pos.x);
    return field_template[y_pos][x_pos];
  }

  bool available_to_move(User& active_user) {
    std::vector<Position> stones = get_all_players_stones(active_user.marker);

    for (auto& pos : stones) {
      for (auto& adj : pos.get_adjacent_positions()) {
        if (get_field_marker_at_position(*adj) == EMPTY_FIELD) {
          return true;
        }
      }
    }
    return false;
  }

  void field_output() {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
      if (i % 2 == 0 && i != FIELD_HEIGHT - 1)
        std::cout << 7 - (i / 2) << "  ";
      else
        std::cout << "   ";
      std::cout << field_template[i] << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds((long long)Helper::ANIMATION_TIME));
    }
  }

  void stone_set() {
    stones_set++;
  }

  // bool player_move_stone(User& active_user, Position& old_pos, Position& new_pos, bool three_stones_left = false) {
  //   // Validate coordinates first
  //   if (!valid_coordinates(old_pos.x, old_pos.y))
  //     throw std::out_of_range(error_msg::INVALID_POSITION);
  //   if (!valid_coordinates(new_pos.x, new_pos.y))
  //     throw std::out_of_range(error_msg::INVALID_POSITION);

  //   // Check if old position has player's stone
  //   if (get_field_marker_at_position(old_pos) != active_user.marker) {
  //     throw std::invalid_argument(error_msg::INVALID_MOVE);
  //   }
  //   if (!three_stones_left && !Position::is_neighbour(old_pos, new_pos))
  //     throw std::invalid_argument(error_msg::NOT_NEIGHBOUR);
  //   // Check if new position is empty
  //   if (get_field_marker_at_position(new_pos) != T) {
  //     throw std::invalid_argument(error_msg::POSITION_OCCUPIED);
  //   }

  //   // Perform the move
  //   int old_y_pos = get_y_pos(old_pos.y);
  //   int old_x_pos = get_x_pos(old_pos.x);
  //   int new_y_pos = get_y_pos(new_pos.y);
  //   int new_x_pos = get_x_pos(new_pos.x);

  //   field_template[old_y_pos][old_x_pos] = T;
  //   field_template[new_y_pos][new_x_pos] = active_user.marker;

  //   return true;
  // }
  void validate_coordinates(Position& pos, char marker) {
    // Validate coordinates and verify position has correct marker
    if (!pos.is_valid()) {
      throw std::out_of_range(error_msg::INVALID_POSITION);
    }

    if (get_field_marker_at_position(pos) != marker) {
      throw std::invalid_argument(error_msg::INVALID_MOVE);
    }
  }

  void player_remove_stone(Position& pos, char marker) {
    // Validate coordinates
    validate_coordinates(pos, marker);

    // Remove the stone
    int y_pos = get_y_pos(pos.y);
    int x_pos = get_x_pos(pos.x);
    field_template[y_pos][x_pos] = EMPTY_FIELD;
  }

  void player_set_stone(Position& pos, char marker) {
    // Validate coordinates
    validate_coordinates(pos, EMPTY_FIELD);

    // Place the stone
    int y_pos = get_y_pos(pos.y);
    int x_pos = get_x_pos(pos.x);
    field_template[y_pos][x_pos] = marker;
  }

  GamePhase get_current_phase() const {
    return current_phase;
  }

  void next_phase() {
    if (current_phase < ENDGAME)
      current_phase = static_cast<GamePhase>(static_cast<int>(current_phase) + 1);
  }

  std::vector<Position> get_all_players_stones(char marker) {

    std::vector<Position> players_positions;

    for (auto pos : get_valid_positions()) {
      if (get_field_marker_at_position(pos) == marker) {
        players_positions.push_back(pos);
      }
    }
    return players_positions;
  }

  std::vector<std::string>& get_field_template() {
    return field_template;
  }

private:
  GamePhase current_phase;
  int stones_set;

  static const int FIELD_WIDTH = 26;
  static const int FIELD_HEIGHT = 15;

  std::vector<std::string> field_template = { // oder so
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

  int get_y_pos(int y_pos) const {
    return ((7 - (y_pos)) * 2);
  }

  int get_x_pos(int x_pos) const {
    return ((x_pos - 1) * 4);
  }
};
#endif