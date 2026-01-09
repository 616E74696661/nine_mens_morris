#include "field.hpp"
#include "mill.hpp"
#include "position.hpp"
#include "user.hpp"
#include <iostream>

class bot_user : public user {
private:
  std::string name = "Bot";

  Position get_random_opponent_position(Field* f) {

    while (true) {
      // Search random positions until an opponent's stone is found
      Position random_pos = get_random_position();
      if (f->get_field_marker_at_position(random_pos) != this->marker &&
          f->get_field_marker_at_position(random_pos) != '#') {
        return random_pos;
      }
    }
  }

public:
  bot_user(std::string name) : user(name) {
  }

  Position place_marker(Field* f) override {

    // Search for potential mills of player to block
    std::pair<Mill, Position> mill_and_pos = Mills::check_potential_mills(f, 'O');
    Position pos = mill_and_pos.second;

    std::cout << this->name << " is placing a marker." << std::endl;

    // If potential mill found, place marker inbetween
    if (pos.is_valid()) {
      std::cout << "Potential mill found." << std::endl;
      return pos;
    }

    // Search for own potential mills to complete
    mill_and_pos = Mills::check_potential_mills(f, this->marker);
    pos = mill_and_pos.second;
    // If potential mill found, place marker to complete mill
    if (pos.is_valid()) {
      std::cout << "Completing own mill." << std::endl;
      return pos;
    }

    // @TODO: Place marker adjacent to own stones
    if (false /*if there are any adjacent positions to own stones*/) {
      // Placeholder for future implementation
    }

    // Else place marker randomly
    std::cout << "No potential mill found, placing marker randomly." << std::endl;
    try {
      return get_random_position();
    } catch (const std::string& e) {
      std::cout << e << std::endl;
      // Retry placing marker
      return place_marker(f);
    }
  }

  void remove_opponent_marker(Field* f) override {
    // If potential mill found, remove one of the player's stones in the mill
    std::cout << this->name << " is removing an opponent's marker." << std::endl;

    std::pair<Mill, Position> mill_and_pos = Mills::check_potential_mills(f, this->marker);
    Position removable_stone;

    // Check if potential mill exists
    if (mill_and_pos.second.is_valid()) {
      // Get stone in potential mill
      for (const auto& mill_pos : mill_and_pos.first) {
        // Skip the free position
        if (mill_pos == mill_and_pos.second) {
          continue;
        }

        char field_marker = f->get_field_marker_at_position(mill_pos);
        if (field_marker != this->marker && field_marker != '#') {
          removable_stone = mill_pos;
          break;
        }
      }
      std::cout << "Removing opponent's stone from potential mill. x:" << removable_stone.x << ", y: " << removable_stone.y << std::endl;

      bool success = f->player_remove_stone(this->marker, &removable_stone);
      if (success)
        return;
      else {
        // Retry removing opponent's marker
        // remove_opponent_marker(f);
        std::cout << "Bot failed to remove stone from potential mill." << std::endl;
        std::exit(1);
      }
    }

    // Else remove random opponent's stone
    try {
      Position random_pos = get_random_opponent_position(f);
      std::cout << "Found opponent's stone at x:" << random_pos.x << ", y: " << random_pos.y << std::endl;

      bool success = f->player_remove_stone(this->marker, &random_pos);
      if (success)
        return;
      else {
        // Retry removing opponent's marker
        // remove_opponent_marker(f);
      }
    } catch (const std::string& e) {
      std::cout << e << std::endl;
      // Retry removing opponent's marker
      // remove_opponent_marker(f);
    }
  }
};