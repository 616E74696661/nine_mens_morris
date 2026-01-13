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
    while (true) { // maybe not while(true) ? you should be able to be in this loop forever, but maybe there's a prettier solution idk
      // Search random positions until an opponent's stone is found
      Position random_pos = get_random_position();
      if (f.get_field_marker_at_position(random_pos) != this->marker &&
          f.get_field_marker_at_position(random_pos) != '#') {
        return random_pos;
      }
    }
  }

  // Position move_stone_to_potential_mill(Field& f, char marker) {
  //   std::pair<Mill, Position> mill_and_pos = Mills::check_potential_mills(f, marker);
  //   if ()
  //     return Position(0, 0);
  // }

public:
  BotUser(std::string name) : User(name) {
  }

  Position place_marker(Field& f, bool moved = false) override {

    std::pair<Mill, Position> mill_and_pos;
    Position pos;
    try {
      // Search for opponents potential mill to block
      mill_and_pos = Mills::check_potential_mills(f, opponent_marker);
      if (pos.is_valid()) {
        // std::cout << "Potential mill found." << std::endl;
        if (f.player_set_stone(*this, pos, moved)) {
          return pos;
        }
      }
      // Search for own potential mill
      mill_and_pos = Mills::check_potential_mills(f, this->marker);
      pos = mill_and_pos.second;
      if (pos.is_valid()) {
        // std::cout << "Completing own mill." << std::endl;
        if (f.player_set_stone(*this, pos, moved)) {
          return pos;
        }
      }
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }

    // TODO: for loop with bot_stones_pos and adjacent positions
    // trying to place a stone near already put stones

    int retries = 10;
    while (retries-- > 0) {
      try {
        pos = get_random_position();
        if (f.player_set_stone(*this, pos, moved)) {
          return pos;
        }
        // std::cout << "Retrying random placement" << std::endl;
      }

      catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
      }
    }

    std::cout << "Failed to place marker" << std::endl;
    return Position();
  }

  /**
   * @brief TODO
   *
   * @param f
   * @param three_stones_left
   * @return Position
   */
  std::pair<Position, Position> move_marker(Field& f, bool three_stones_left = false) override {
    // while (true) {
    //   Position pos = get_random_position();
    //   Position new_pos = get_random_position();
    //   if (!three_stones_left && !pos.is_neighbour(pos, new_pos))
    //     continue;
    //   if (f.player_move_stone(this->marker, pos, new_pos)) {
    //     return new_pos;
    //   }
    // }
    std::pair<Mill, Position> mill_and_pos;
    Position pos;

    // check if pos even has an empty neighbour before when not endgame

    try {
      // search for own potential mill
      mill_and_pos = Mills::check_potential_mills(f, this->marker);
      pos = mill_and_pos.second;
      if (pos.is_valid()) {
        std::vector<Position*> neighbours = pos.get_neighbours();
        for (auto neighbour : neighbours) {
          // if neighbour is own stone, move stone to pos
          if (f.get_field_marker_at_position(*neighbour) == this->marker) {
            bool success = f.player_set_stone(*this, *neighbour, true);
            if (success)
              return std::pair<Position, Position>(pos, *neighbour);
          }
        }
      }

      if (f.player_remove_stone(*this, pos)) {
        try {
          std::cout << "New position:" << std::endl;
          unsigned int new_y_pos = Helper::read_uint("y: ");
          unsigned int new_x_pos = Helper::read_uint("x: ");
          Position new_pos = Position(new_x_pos, new_y_pos);
          // ?????????
          if (!three_stones_left && !pos.is_neighbour(pos, new_pos))
            throw std::invalid_argument(error_msg::INVALID_SELECTION);
          bool success = f.player_set_stone(*this, new_pos, true);
          if (success)
            return std::pair<Position, Position>(pos, new_pos);
        } catch (const std::exception& e) {
          f.player_set_stone(*this, pos, true);
        }
      }
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }
    return std::pair<Position, Position>(Position(), Position());
  }

  Position remove_opponent_marker(Field& f, User& other) override {
    Position removable_stone;
    std::pair<Mill, Position> mill_and_pos;
    Mill mill;
    Position pos;
    while (true) {
      try {
        // Search for potential closed mill of opponent to block
        mill_and_pos = Mills::check_potential_mills(f, this->marker);
        mill = mill_and_pos.first;
        pos = mill_and_pos.second;

        if (pos.is_valid()) {
          // search opponents stone
          for (std::size_t i = 0; i < mill.size(); ++i) {
            if (mill[i] == pos)
              continue;

            char field_marker = f.get_field_marker_at_position(mill[i]);
            if (field_marker != this->marker && field_marker != '#') {
              removable_stone = mill[i];
              break;
            }
          }
        }

        removable_stone = get_random_opponent_position(f);

        bool success = f.opponent_remove_stone(this->marker, removable_stone, other);
        if (success)
          break;
      } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
      }
    }
    return removable_stone;
  }
};