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
    std::cout << "Select marker position:" << std::endl;

    int pos_y;
    int pos_x;

    try {
      std::cout << "y: ";
      std::cin >> pos_y;
      std::cout << "x: ";
      std::cin >> pos_x;
      Position pos(pos_x, pos_y);
      return pos;
    } catch (const std::string& e) {
      std::cout << e << std::endl;
      // Retry placing marker
      return place_marker();
    }
  }
};