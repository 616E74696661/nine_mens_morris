#ifndef MILL_HPP
#define MILL_HPP

#include "field.hpp"
#include "position.hpp"
#include <algorithm>
#include <array>
#include <utility>

using Mill = std::array<Position, 3>;

static const std::array<Mill, 16> valid_mills_array{
    {{Position{1, 1}, Position{1, 4}, Position{1, 7}},
     {Position{2, 2}, Position{2, 4}, Position{2, 6}},
     {Position{3, 3}, Position{3, 4}, Position{3, 5}},
     {Position{4, 1}, Position{4, 2}, Position{4, 3}},
     {Position{4, 5}, Position{4, 6}, Position{4, 7}},
     {Position{5, 3}, Position{5, 4}, Position{5, 5}},
     {Position{6, 2}, Position{6, 4}, Position{6, 6}},
     {Position{7, 1}, Position{7, 4}, Position{7, 7}},
     {Position{1, 1}, Position{4, 1}, Position{7, 1}},
     {Position{2, 2}, Position{4, 2}, Position{6, 2}},
     {Position{3, 3}, Position{4, 3}, Position{5, 3}},
     {Position{1, 4}, Position{2, 4}, Position{3, 4}},
     {Position{5, 4}, Position{6, 4}, Position{7, 4}},
     {Position{3, 5}, Position{4, 5}, Position{5, 5}},
     {Position{2, 6}, Position{4, 6}, Position{6, 6}},
     {Position{1, 7}, Position{4, 7}, Position{7, 7}}}};

class Mills {
public:
  // Compares two mills for equality, no matter the order of positions
  static bool millsEqual(Mill a, Mill b) {
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    return a == b;
  }

  // Checks if a mill is valid
  static bool isValidMill(const Mill& mill) {
    for (const auto& validMill : valid_mills_array) {
      if (millsEqual(mill, validMill)) {
        return true;
      }
    }
    return false;
  }

  // Checks if a mill is formed at the given position for the given marker
  static bool check_mill(const Position& pos, char marker, Field& f) {
    // Loops through all valid mills
    for (const auto& mill : valid_mills_array) {
      // If the mill includes the position
      if (std::find(mill.begin(), mill.end(), pos) != mill.end()) {
        // Check if the mill is formed
        if (f.check_mill(mill, marker)) {
          return true;
        }
      }
    }
    return false;
  }

  static std::pair<Mill, Position> check_potential_mills(Field& f, char marker) {
    // Returns a mill that can be completed and the free position to complete it

    for (const auto& mill : valid_mills_array) {
      unsigned int user_in_question_stones = 0;
      unsigned int other_user_stones = 0;
      Position free_pos = Position();

      // Check every possible mill for completable mills
      for (Position pos : mill) {
        char field_marker = f.get_field_marker_at_position(pos);
        if (field_marker == marker) {
          user_in_question_stones++;
        } else if (field_marker != '#') {
          other_user_stones++;
        } else {
          free_pos = pos;
        }
      }
      // If a mill can be completed, return the free position
      if (user_in_question_stones == 2 && other_user_stones == 0) {
        return std::pair<Mill, Position>(mill, free_pos);
      }
    }

    // No potential mill found, returning invalid position
    return std::pair<Mill, Position>(Mill(), Position());
  }

  static bool pos_is_part_of_mill(Position& target_pos, Mill& mill) {
    for (auto pos : mill) {
      if (pos == target_pos)
        return true;
    }
    return false;
  }
};

#endif
