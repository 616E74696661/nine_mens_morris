#include "field.hpp"
#include "mill.hpp"
#include "position.hpp"
#include "user.hpp"
#include <iostream>

class BotUser : public User {
private:
  std::string name = "Bot";

  Position get_random_opponent_position(Field& f) {
    while (true) {
      // Search random positions until an opponent's stone is found
      Position random_pos = get_random_position();
      if (f.get_field_marker_at_position(random_pos) != this->marker &&
          f.get_field_marker_at_position(random_pos) != '#') {
        return random_pos;
      }
    }
  }

public:
  BotUser(std::string name) : User(name) {
  }

  Position place_marker(Field& f) override {

    std::pair<Mill, Position> mill_and_pos;
    Position pos;
    try {
      // Search for opponents potential mill to block
      mill_and_pos = Mills::check_potential_mills(f, 'O');
      pos = mill_and_pos.second;
      if (pos.is_valid()) {
        std::cout << "Potential mill found." << std::endl;
        if (f.player_set_stone(*this, pos)) {
          return pos;
        }
      }
      // Search for own potential mill to block
      mill_and_pos = Mills::check_potential_mills(f, this->marker);
      pos = mill_and_pos.second;
      if (pos.is_valid()) {
        std::cout << "Completing own mill." << std::endl;
        if (f.player_set_stone(*this, pos)) {
          return pos;
        }
      }
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }

    // TODO: for loop with bot_stones_pos and adjacent positions
    // trying to place a stone near already put stones

    int retries = 10;
    while (retries > 0) {
      try {
        pos = get_random_position();
        if (f.player_set_stone(*this, pos)) {
          return pos;
        }
        std::cout << "Retrying random placement" << std::endl;
      }

      catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
      }
      retries--;
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
  std::pair<Position, Position>
  move_marker(Field& f, bool three_stones_left = false) override {

    // while (true) {
    //   Position pos = get_random_position();
    //   Position new_pos = get_random_position();

    //   if (!three_stones_left && !pos.is_neighbour(pos, new_pos))
    //     continue;
    //   if (f.player_move_stone(this->marker, pos, new_pos)) {
    //     return new_pos;
    //   }
    // }

    return std::pair<Position, Position>(Position(), Position());
  }

  Position remove_opponent_marker(Field& f, User& other) override {
    Position removable_stone;
    while (true) {
      try {
        // Search for potential closed mill of player to block
        std::pair<Mill, Position> mill_and_pos = Mills::check_potential_mills(f, this->marker);
        Mill mill = mill_and_pos.first;
        Position pos = mill_and_pos.second;

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