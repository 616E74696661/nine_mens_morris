#include "field.hpp"
#include "mill.hpp"
#include "position.hpp"
#include "user.hpp"
#include <exception>
#include <iostream>
#include <stdexcept>

class BotUser : public User {
private:
  std::string name = "Bot";
  char opponent_marker = 'O';

  /**
   * @brief get a random stone position of opponent
   *
   * @param f Field
   * @return Position
   */
  Position get_random_opponent_position(Field& f) {
    Position random_pos;
    do
      random_pos = get_random_position();
    while (f.get_field_marker_at_position(random_pos) == this->marker ||
           f.get_field_marker_at_position(random_pos) == '#');
    return random_pos;
  }

  // Position move_stone_to_potential_mill(Field& f, char marker) {
  //   std::pair<Mill, Position> mill_and_pos = Mills::check_potential_mills(f, marker);
  //   if ()
  //     return Position(0, 0);
  // }

public:
  BotUser(std::string name) : User(name) {}

  Position place_marker(Field& f, bool moved = false) override {
    std::pair<Mill, Position> mill_and_pos;
    Position pos;
    try {
      // block opponent's potential mill
      mill_and_pos = Mills::check_potential_mills(f, opponent_marker);
      pos = mill_and_pos.second;
      if (pos.is_valid() && f.player_set_stone(*this, pos, moved))
        return pos;

      // complete own potential mill
      mill_and_pos = Mills::check_potential_mills(f, this->marker);
      pos = mill_and_pos.second;
      if (pos.is_valid() && f.player_set_stone(*this, pos, moved))
        return pos;
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }

    // fallback: random placement
    while (true) {
      try {
        pos = get_random_position();
        if (f.player_set_stone(*this, pos, moved))
          return pos;
      } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
      }
    }

    std::cout << "Failed to place marker" << std::endl;
    return Position();
  }

  /**
   * @brief Move a marker on the field
   *
   * @param f Field
   * @param three_stones_left Whether player has only 3 stones (= can jump)
   * @return std::pair<Position, Position> Old and new positions
   */
  std::pair<Position, Position> move_marker(Field& f, bool three_stones_left = false) override {
    try {
      // try to complete own potential mill
      std::pair<Mill, Position> mill_and_pos = Mills::check_potential_mills(f, this->marker);
      Position target_pos = mill_and_pos.second;
      if (target_pos.is_valid()) {
        if (!three_stones_left) {
          std::vector<Position*> neighbours = target_pos.get_neighbours();
          for (auto neighbour : neighbours) {
            if (f.get_field_marker_at_position(*neighbour) == this->marker) {
              // remove stone from current position
              if (f.player_remove_stone(*this, *neighbour)) {
                // try to place at target position
                if (f.player_set_stone(*this, target_pos, true))
                  return std::make_pair(*neighbour, target_pos);
                // rollback if placement failed
                f.player_set_stone(*this, *neighbour, true);
              }
            }
          }
        } else {
          // TODO
        }
      }
      // fallback: random move
      int retries = 10;
      while (retries-- > 0) {
        Position from_pos = get_random_position();
        Position to_pos = get_random_position();
        // check if from_pos is our stone
        if (f.get_field_marker_at_position(from_pos) != this->marker)
          continue;
        // check adjacency constraint (unless we can jump with 3 stones)
        if (!three_stones_left && !from_pos.is_neighbour(from_pos, to_pos))
          continue;
        // try the move
        if (f.player_remove_stone(*this, from_pos))
          if (f.player_set_stone(*this, to_pos, true))
            return std::make_pair(from_pos, to_pos);
        // rollback
        f.player_set_stone(*this, from_pos, true);
      }
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }
    return std::pair<Position, Position>(Position(), Position());
  }

  Position remove_opponent_marker(Field& f, User& other) override {
    Position removable_stone;

    while (true) {
      try {
        // try to block opponent's potential mill
        std::pair<Mill, Position> mill_and_pos = Mills::check_potential_mills(f, this->marker);
        Mill mill = mill_and_pos.first;
        Position pos = mill_and_pos.second;

        if (pos.is_valid())
          for (std::size_t i = 0; i < mill.size(); ++i) {
            if (mill[i] == pos)
              continue;

            char field_marker = f.get_field_marker_at_position(mill[i]);
            if (field_marker != this->marker && field_marker != '#') {
              removable_stone = mill[i];
              break;
            }
          }

        // fallback: random opponent stone
        if (!removable_stone.is_valid())
          removable_stone = get_random_opponent_position(f);

        if (f.opponent_remove_stone(this->marker, removable_stone, other))
          break;
      } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
      }
    }
    return removable_stone;
  }
};