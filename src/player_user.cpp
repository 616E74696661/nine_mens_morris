#include "mills.cpp"
#include "position.hpp"
#include "user.cpp"
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

  void remove_opponent_marker(Mills* m) override {
    std::cout << "You formed a mill! Select an opponent's marker to remove:" << std::endl;

    int pos_y;
    int pos_x;

    try {
      std::cout << "y: ";
      std::cin >> pos_y;
      std::cout << "x: ";
      std::cin >> pos_x;
      Position pos(pos_x, pos_y);
      bool success = m->player_remove_stone(this->marker, &pos);
      if (!success) {
        // Retry removing opponent's marker
        remove_opponent_marker(m);
      }
    } catch (const std::string& e) {
      std::cout << e << std::endl;
      // Retry removing opponent's marker
      remove_opponent_marker(m);
    }
  }
};