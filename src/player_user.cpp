#include "constants/error_messages.hpp"
#include "helper.hpp"
#include "position.hpp"
#include "user.hpp"
#include <iostream>
#include <string>

class player_user : public user {
public:
  player_user(std::string name) : user(name) {
  }

  Position place_marker() override {
    std::cout << "It's your turn, " << this->name << "!" << std::endl;
    std::cout << "Select stone position:" << std::endl;

    int y_pos;
    int x_pos;

    try {
      y_pos = read_int("y: ");
      x_pos = read_int("x: ");
      Position pos(x_pos, y_pos);
      return pos;
    } catch (const std::string& e) {
      std::cout << e << std::endl;
      // Retry placing marker
      return place_marker();
    }
  }

  std::pair<Position, Position> move_marker() override {
    std::cout << "It's your turn, " << this->name << "!" << std::endl;
    std::cout << "Select stone you want to move:" << std::endl;

    int y_pos;
    int x_pos;
    Position pos;
    try {
      y_pos = read_int("y: ");
      x_pos = read_int("x: ");
      pos = Position(x_pos, y_pos);
    } catch (const std::string& e) {
      std::cout << e << std::endl;
      // Retry moving stone
      return move_marker();
    }

    std::cout << "New position:" << std::endl;

    int new_y_pos;
    int new_x_pos;
    Position new_pos;

    try {
      new_y_pos = read_int("y: ");
      new_x_pos = read_int("x: ");
      new_pos = Position(new_x_pos, new_y_pos);

      if (pos.is_neighbour(pos, new_pos))
        return std::make_pair(pos, new_pos);
      else
        throw error_msg::INVALID_SELECTION;
    } catch (const std::string& e) {
      std::cout << e << std::endl;
      // Retry moving stone
      return move_marker();
    }
  }

  std::pair<Position, Position> remove_opponent_marker() override {
    return std::make_pair(Position(), Position());
  }
};