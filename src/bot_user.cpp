#include "Mills.hpp"
#include "position.hpp"
#include "user.hpp"
#include <iostream>

class bot_user : public user {
private:
  std::string name = "Bot";

  std::unique_ptr<Position> check_potential_mills(Mills* m) {
    for (const auto& mill : valid_mills) {
      int bot_stones = 0;
      int player_stones = 0;
      Position free_pos = Invalid_Position();

      // Check every possible mill for completable mills
      for (const auto& pos : mill) {

        if (m->get_field_marker_at_position(pos) == 'O') {
          player_stones++;
        } else if (m->get_field_marker_at_position(pos) == 'X') {
          bot_stones++;
        } else {
          free_pos = Position(pos.first, pos.second);
        }
      }
      // If a mill can be completed, return the free position
      if ((player_stones == 2 && bot_stones == 0) || (bot_stones == 2 && player_stones == 0)) {
        return std::make_unique<Position>(free_pos);
      }
    }
    return std::make_unique<Invalid_Position>();
  }

public:
  bot_user(std::string name) : user(name) {
  }

  Position place_marker(Mills* m) override {

    auto pos = check_potential_mills(m);
    std::cout << this->name << " is placing a marker." << std::endl;

    // If potential mill found, place marker inbetween
    if (!dynamic_cast<Invalid_Position*>(pos.get())) {
      std::cout << "Potential mill found." << std::endl;
      return *pos;
    }

    // Else place marker randomly
    try {
      return get_random_position();
    } catch (const std::string& e) {
      std::cout << e << std::endl;
      // Retry placing marker
      return place_marker(m);
    }
  }
};