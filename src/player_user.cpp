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
  PlayerUser(std::string name, int set = 0, int removed = 0) : User(name, set, removed) {}

  Position place_marker(Field& f) override {
    set_stone();

    while (true) {
      std::cout << "Select stone position:" << std::endl;
      try {
        int y_pos = Helper::read_uint("y: ");
        int x_pos = Helper::read_uint("x: ");
        Position pos(x_pos, y_pos);

        f.player_set_stone(pos, marker);
        return pos;

      } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
      }
    }
  }

  Position remove_opponent_marker(Field& f) override {
    std::cout << "You formed a mill! Select an opponent's marker to remove:" << std::endl;

    while (true) {
      try {
        int y_pos = Helper::read_uint("y: ");
        int x_pos = Helper::read_uint("x: ");
        Position pos(x_pos, y_pos);

        if (pos.is_valid()) {
          f.player_remove_stone(pos, opponent_marker);
          return pos;
        }
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
        int y_pos = Helper::read_uint("y: ");
        int x_pos = Helper::read_uint("x: ");
        Position pos = Position(x_pos, y_pos);

        f.player_remove_stone(pos, marker);
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
        int y_pos = Helper::read_uint("y: ");
        int x_pos = Helper::read_uint("x: ");
        Position new_pos = Position(x_pos, y_pos);

        if (three_stones_left || Position::is_adjacent_position(*old_pos, new_pos)) {
          f.player_set_stone(new_pos, marker);
          return std::pair<Position, Position>(*old_pos, new_pos);
        }
        throw std::invalid_argument(error_msg::INVALID_SELECTION);
      } catch (const std::exception& e) {
        std::cout << "Error at moving stone: Failed to set stone." << std::endl;
        std::cout << e.what() << std::endl;

        // Rollback necessary
        try {
          f.player_set_stone(*old_pos, marker);
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