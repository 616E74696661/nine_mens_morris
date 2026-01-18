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
  static const char EMPTY_FIELD = '#'; // Marker for a empty position
  Field(std::vector<std::string> field_string = {}) {
    if (!field_string.empty())
      field_template = field_string;
  }

  /**
   * @brief Get the field marker at the given position
   *
   * @param pos
   * @return char
   */
  char get_field_marker_at_position(Position& pos) {
    if (!pos.is_valid()) {
      throw std::out_of_range(error_msg::INVALID_POSITION);
    }

    int y_pos = get_y_pos(pos.y);
    int x_pos = get_x_pos(pos.x);
    return field_template[y_pos][x_pos];
  }

  /**
   * @brief Checks whether a player is able to move at least one of it's stones
   *
   * @param active_user User to check
   * @return true User is able to move a stone
   * @return false User is unable to move a stone
   */
  bool available_to_move(User& active_user) {
    std::vector<Position> stones = get_all_players_stones(active_user.marker);

    for (auto& pos : stones) {
      for (auto& adj : pos.get_adjacent_positions()) {
        if (get_field_marker_at_position(adj) == EMPTY_FIELD) {
          return true;
        }
      }
    }
    return false;
  }

  /**
   * @brief Print the current state of the gameboard
   *
   */
  void field_output() {
    for (int i = 0; i < field_template.size(); i++) {
      if (i % 2 == 0 && i != field_template[0].size() - 1)
        std::cout << 7 - (i / 2) << "  ";
      else
        std::cout << "   ";
      std::cout << field_template[i] << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds((long long)Helper::ANIMATION_TIME));
    }
  }

  /**
   * @brief Check the if the position is valid and if the position is occupied by the specified marker
   *
   * @param pos
   * @param marker
   */
  void validate_coordinates(Position& pos, char marker) {
    // Validate coordinates and verify position has correct marker
    if (!pos.is_valid()) {
      throw std::out_of_range(error_msg::INVALID_POSITION);
    }

    if (get_field_marker_at_position(pos) != marker) {
      throw std::invalid_argument(error_msg::INVALID_MOVE);
    }
  }

  /**
   * @brief Removes a marker from a given position on the game board
   *
   * @param pos
   * @param marker
   */
  void player_remove_stone(Position& pos, char marker) {
    // Validate coordinates
    validate_coordinates(pos, marker);

    // Remove the stone
    int y_pos = get_y_pos(pos.y);
    int x_pos = get_x_pos(pos.x);
    field_template[y_pos][x_pos] = EMPTY_FIELD;
  }

  /**
   * @brief Sets a marker on a given position on the game board
   *
   * @param pos
   * @param marker
   */
  void player_set_stone(Position& pos, char marker) {
    // Validate coordinates
    validate_coordinates(pos, EMPTY_FIELD);

    // Place the stone
    int y_pos = get_y_pos(pos.y);
    int x_pos = get_x_pos(pos.x);
    field_template[y_pos][x_pos] = marker;
  }

  /**
   * @brief Get the all players stones
   *
   * @param marker Marker to get stones of
   * @return std::vector<Position>
   */
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
  /**
   * @brief Template of the game board
   *
   */
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

  /**
   * @brief Get the y-position of the gameboard template based on the y-input-position
   *
   * @param y_pos
   * @return int
   */
  int get_y_pos(int y_pos) const {
    return ((7 - (y_pos)) * 2);
  }

  /**
   * @brief Get the x-position of the gameboard template based on the x-input-position
   *
   * @param x_pos
   * @return int
   */
  int get_x_pos(int x_pos) const {
    return ((x_pos - 1) * 4);
  }
};
#endif