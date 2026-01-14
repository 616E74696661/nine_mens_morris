#include "constants/error_messages.hpp"
#include "field.hpp"
#include "helper.hpp"
#include "position.hpp"
#include "user.hpp"
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

  /**
   * @brief
   * TODO: check if even able to move the selected stone. if not -> instant throw
   * @param f
   * @param three_stones_left
   * @return Position
   */
  std::pair<Position, Position> move_marker(Field& f, bool three_stones_left = false) override {
    while (true) {
      std::cout << "Select stone you want to move:" << std::endl;
      try {
        unsigned int y_pos = Helper::read_uint("y: ");
        unsigned int x_pos = Helper::read_uint("x: ");
        Position pos = Position(x_pos, y_pos);

        if (f.player_remove_stone(*this, pos)) {
          try {
            std::cout << "New position:" << std::endl;
            unsigned int new_y_pos = Helper::read_uint("y: ");
            unsigned int new_x_pos = Helper::read_uint("x: ");
            Position new_pos = Position(new_x_pos, new_y_pos);

            // Check adjacency constraint if not in endgame
            if (!three_stones_left && !Position::is_neighbour(pos, new_pos)) {
              throw std::invalid_argument(error_msg::INVALID_SELECTION);
            }

            bool success = f.player_set_stone(*this, new_pos, true);
            if (success) {
              return std::make_pair(pos, new_pos);
            }

          } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            // Rollback: put the stone back - wrap in try-catch to handle invalid pos
            try {
              f.player_set_stone(*this, pos, true);
            } catch (...) {
              // If rollback fails, the stone is already removed from the field
              // This shouldn't happen in normal gameplay
            }
          }
        }
      } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
      }
    }
  }
};