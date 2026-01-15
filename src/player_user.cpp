#include "constants/error_messages.hpp"
#include "field.hpp"
#include "helper.hpp"
#include "position.hpp"
#include "user.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

class PlayerUser : public User {
public:
  PlayerUser(std::string name) : User(name) {
  }

  Position place_marker(Field& f, bool moved = false) override {
    while (true) {
      std::cout << "Select stone position:" << std::endl;
      try {
        unsigned int y_pos = Helper::read_uint("y: ");
        unsigned int x_pos = Helper::read_uint("x: ");
        Position pos(x_pos, y_pos);
        if (f.player_set_stone(*this, pos, moved)) {
          return pos;
        }
      } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
      }
    }
  }

  Position remove_opponent_marker(Field& f, User& other) override {
    std::cout << "You formed a mill! Select an opponent's marker to remove:" << std::endl;

    while (true) {
      try {
        unsigned int y_pos = Helper::read_uint("y: ");
        unsigned int x_pos = Helper::read_uint("x: ");
        Position pos(x_pos, y_pos);
        bool success = f.opponent_remove_stone(this->marker, pos, other);
        if (success)
          return pos;
      } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
      }
    }
  }

  std::pair<Position, Position> move_marker(Field& f, bool three_stones_left) override {
    while (true) {
      // select stone to move
      Position* old_pos;

      try {
        std::cout << "Select stone you want to move:" << std::endl;
        unsigned int y_pos = Helper::read_uint("y: ");
        unsigned int x_pos = Helper::read_uint("x: ");
        Position pos = Position(x_pos, y_pos);

        f.player_remove_stone(*this, pos);
        old_pos = &pos;

      } catch (const std::exception& e) {
        std::cout << "Error at moving stone: Failed to remove stone." << std::endl;
        std::cout << e.what() << std::endl;
        continue; // restart loop
      }

      // if we reach this point, stone was successfully removed

      // select new position to move stone to
      try {
        std::cout << "Select new position:" << std::endl;
        unsigned int y_pos = Helper::read_uint("y: ");
        unsigned int x_pos = Helper::read_uint("x: ");
        Position new_pos = Position(x_pos, y_pos);

        if (three_stones_left || Position::is_adjacent_position(*old_pos, new_pos)) {
          f.player_set_stone(*this, new_pos);
          return std::pair<Position, Position>(*old_pos, new_pos);
        }
        throw std::invalid_argument(error_msg::INVALID_SELECTION);
      } catch (const std::exception& e) {
        std::cout << "Error at moving stone: Failed to set stone." << std::endl;
        std::cout << e.what() << std::endl;

        // Rollback necessary
        try {
          f.player_set_stone(*this, *old_pos);
        } catch (const std::exception& e) {
          std::cout << "Error at rollback during player's move. Quitting program" << std::endl;
          std::exit(1);
        }
      }
    }
  }

  bool is_bot() override {
    return false;
  }
};