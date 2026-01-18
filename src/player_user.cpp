#include "constants/error_messages.hpp"
#include "field.hpp"
#include "helper.hpp"
#include "mill.hpp"
#include "position.hpp"
#include "user.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class PlayerUser : public User {
public:
  /**
   * @brief Construct a new Player User object
   *
   * @param name Name of the Player
   */
  PlayerUser(std::string name) : User(name) {
  }

  /**
   * @brief Place a marker on the gameboard
   *
   * @param f The gameboard
   * @return The position the marker has been placed on
   */
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

  /**
   * @brief Remove one of the opponent's markers
   *
   * @param f The gamegoard
   * @return Position of the removed marker
   */
  Position remove_opponent_marker(Field& f) override {
    std::cout << "You formed a mill! Select an opponent's marker to remove:" << std::endl;

    while (true) {
      try {
        int y_pos = Helper::read_uint("y: ");
        int x_pos = Helper::read_uint("x: ");
        Position pos(x_pos, y_pos);

        if (pos.is_valid()) {

          std::vector<Position> removeable_stones = Mills::get_removeable_stones(f, opponent_marker);
          if (!contains(removeable_stones, pos)) {
            throw std::invalid_argument(error_msg::STONE_PART_OF_MILL);
          }
          f.player_remove_stone(pos, opponent_marker);
          return pos;
        }
      } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
      }
    }
  }

  /**
   * @brief Move a stones in from one position to another
   *
   * @param f The field with the current gamestate
   * @param jump_allowed A boolean which decides whether a player is able to jump with it's stones or not
   * @return std::pair<Position, Position> The old position where the marker has been and the new position where it has been moved to
   */
  std::pair<Position, Position> move_marker(Field& f, bool jump_allowed) override {

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

      // Select new position to move stone to
      try {
        std::cout << "Select new position:" << std::endl;
        int y_pos = Helper::read_uint("y: ");
        int x_pos = Helper::read_uint("x: ");
        Position new_pos = Position(x_pos, y_pos);

        if (jump_allowed || Position::is_adjacent_position(*old_pos, new_pos)) {
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

  /**
   * @brief Returns a boolean which says whether the player is a bot or not
   *
   * @return true if player is a bot
   * @return false if player is a human player
   */
  bool is_bot() override {
    return false;
  }
};