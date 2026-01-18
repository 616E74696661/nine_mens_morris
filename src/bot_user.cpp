#include "field.hpp"
#include "mill.hpp"
#include "position.hpp"
#include "user.hpp"
#include <exception>
#include <float.h>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

class BotUser : public User {
private:
  /**
   * @brief Get a random position of the marker's stones
   *
   * @param f Gameboard
   * @param marker Marker to search stoens of
   * @param available_positions Optional vector to get random position from
   * @return Random position
   */
  Position get_random_position(Field& f, char marker, std::vector<Position> available_positions = {}) {
    std::vector<Position> positions;
    if (available_positions.empty()) {
      positions = f.get_all_players_stones(marker);
    } else {
      positions = available_positions;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, positions.size() - 1);
    int randomNumber = (int)dist(gen);

    return positions[randomNumber];
  }

  /**
   * @brief Get stone to move to target position and prevent / complete mill
   *
   * @param f Gameboard
   * @param target_pos The position to move to
   * @param mill The potential mill to be prevented / completed
   * @param jump_allowed Boolean to decide whether you can jump with your stones
   * @return Position of the stone to move to the target position. If no stone was found, return invalid position
   */
  Position select_stone_to_fill_mill(Field& f, Position* target_pos, Mill* mill, bool jump_allowed) {

    std::cout << "Jump allowed: " << jump_allowed << std::endl;

    if (jump_allowed) {
      // select stone which is not included within the potential mill
      for (auto pos : f.get_all_players_stones(marker)) {
        if (!Mills::pos_is_part_of_mill(pos, *mill)) {
          return pos;
        }
      }
    }

    // Search for adjacent stone to the target position
    std::vector<Position*> adj_positions = target_pos->get_adjacent_positions();
    for (auto pos : adj_positions) {
      if (!Mills::pos_is_part_of_mill(*pos, *mill) && f.get_field_marker_at_position(*pos) == marker) {
        // Position is not part of the mill and occupied by this bot
        return *pos;
      }
    }

    // No stone found to block the marker -> select random to move random
    // Return values ändern -> erkennbar, ob mill geblockt werden kann
    // return invalid Position, False
    std::cout << "Retuning invalid position" << std::endl;
    return Position();
  }

  /**
   * @brief Returns a move. This includes the old and new position of a stone. First try to complete own mill, then try to prevent opponent's mill otherwise return random move
   *
   * @param f The gameboard
   * @param jump_allowed Boolean to decide whether you can jump with your stones
   * @return std::pair<Position, Position> Old position and new position of the stone moved
   */
  std::pair<Position, Position> get_move(Field& f, bool jump_allowed) {

    // try to complete own potential mill
    std::pair<Mill, Position> mill_and_pos = Mills::check_potential_mills(f, this->marker);
    Mill target_mill = mill_and_pos.first;
    Position target_pos = mill_and_pos.second;

    Position moveable_stone;

    if (target_pos.is_valid()) {
      std::cout << "Trying to complete own mill" << std::endl;
      moveable_stone = select_stone_to_fill_mill(f, &target_pos, &target_mill, jump_allowed);

      if (moveable_stone.is_valid()) {
        f.validate_coordinates(moveable_stone, marker);
        return std::pair<Position, Position>(moveable_stone, target_pos);
      }
    }

    // try to block opponents potential mill
    mill_and_pos = Mills::check_potential_mills(f, opponent_marker);
    target_mill = mill_and_pos.first;
    target_pos = mill_and_pos.second;

    if (target_pos.is_valid()) {
      std::cout << "Trying to block opponent's mill" << std::endl;
      moveable_stone = select_stone_to_fill_mill(f, &target_pos, &target_mill, jump_allowed);
      if (moveable_stone.is_valid()) {
        f.validate_coordinates(moveable_stone, marker);
        return std::pair<Position, Position>(moveable_stone, target_pos);
      }
    }

    // No stone was found to either complete or prevent a mill

    // Random move
    std::cout << "Random move" << std::endl;
    if (jump_allowed) {
      // return random stone to jump to an empty position
      Position old_pos = get_random_position(f, this->marker);
      Position new_pos = get_random_position(f, f.EMPTY_FIELD);
      return std::pair<Position, Position>(old_pos, new_pos);
    } else {
      // Search for stone with empty neighbour
      std::vector<Position> own_stones = f.get_all_players_stones(marker);
      for (auto pos : own_stones) {
        for (auto neighbour : pos.get_adjacent_positions()) {
          if (f.get_field_marker_at_position(*neighbour) == f.EMPTY_FIELD) {
            return std::pair<Position, Position>(pos, *neighbour);
          }
        }
      }
    }

    // Programm should not be able to get here
    throw std::runtime_error("Reached unexpected point in code");
  }

public:
  /**
   * @brief Construct a new Bot User
   *
   * @param name
   */
  BotUser(std::string name) : User(name) {}

  /**
   * @brief Bot places a marker. First trying to prevent opponent's mill, then try to complete own mill otherwise place randomly
   *
   * @param f The gameboard
   * @return Position the stone was set on
   */
  Position place_marker(Field& f) override {

    set_stone();

    std::pair<Mill, Position> mill_and_pos;
    Position pos;
    try {
      // block opponent's potential mill
      mill_and_pos = Mills::check_potential_mills(f, opponent_marker);
      pos = mill_and_pos.second;

      // complete own potential mill
      if (!pos.is_valid()) {
        mill_and_pos = Mills::check_potential_mills(f, this->marker);
        pos = mill_and_pos.second;
      }
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }

    // fallback: random placement
    if (!pos.is_valid()) {
      pos = get_random_position(f, f.EMPTY_FIELD);
    }
    f.player_set_stone(pos, marker);
    return pos;
  }

  /**
   * @brief Bot moves a marker.
   *
   * @param f
   * @param three_stones_left
   * @return std::pair<Position, Position>
   */
  std::pair<Position, Position> move_marker(Field& f, bool three_stones_left) override {

    // Get move to make
    std::pair<Position, Position> move;
    try {
      move = get_move(f, three_stones_left);
    } catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }

    Position old_pos = move.first;
    Position new_pos = move.second;

    // Update board
    f.player_remove_stone(old_pos, marker);
    f.player_set_stone(new_pos, marker);

    return move;
  }

  /**
   * @brief Bot removes player's stone
   *
   * @param f The gameboard
   * @return Position of the removed marker
   */
  Position remove_opponent_marker(Field& f) override {

    // random opponent stone
    std::vector<Position> removeable_stones = Mills::get_removeable_stones(f, opponent_marker);
    Position removable_stone = get_random_position(f, opponent_marker, removeable_stones);

    // try to block opponent's potential mill
    try {
      std::pair<Mill, Position> mill_and_pos = Mills::check_potential_mills(f, opponent_marker);
      Mill mill = mill_and_pos.first;
      Position free_pos = mill_and_pos.second;

      if (free_pos.is_valid()) {
        // check if potential mill found
        for (auto& pos : mill) {
          if (pos == free_pos) {
            continue;
          }

          if (contains(removeable_stones, pos)) {
            removable_stone = pos;
            break;
          }
        }
      }
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }

    f.player_remove_stone(removable_stone, opponent_marker);
    return removable_stone;
  }

  /**
   * @brief Returns a boolean which says whether the player is a bot or not
   *
   * @return true if player is a bot
   * @return false if player is a human player
   */
  bool is_bot() override {
    return true;
  }
};