#ifndef MILL_HPP
#define MILL_HPP

#include "field.hpp"
#include "position.hpp"
#include "set"
#include <algorithm>
#include <array>
#include <utility>

/**
 * @brief Alias for a mill containing exactly three Position elements.
 *
 */
using Mill = std::array<Position, 3>;

/**
 * @brief Array containing every possible mill
 *
 */
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
  /**
   * @brief Compares two mills for equality, no matter the order of positions

   *
   * @param a First mill
   * @param b Second mill
   * @return true Mills equal each other
   * @return false Mills are not equal
   */
  static bool millsEqual(Mill a, Mill b) {
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    return a == b;
  }

  /**
   * @brief Checks if a mill is formed at the given position for the given marker

   *
   * @param pos Latest stone set on the board
   * @param marker The marker to check the mill for
   * @param f The board
   * @return true A mill has been completed
   * @return false No mill has been completed
   */
  static bool check_mill(const Position& pos, char marker, Field& f) {
    // Loops through all valid mills
    for (const auto& mill : valid_mills_array) {

      // If the mill includes the position
      if (std::find(mill.begin(), mill.end(), pos) != mill.end()) {
        // Check if the mill is formed
        int counter = 0;
        for (auto pos : mill) {
          if (f.get_field_marker_at_position(pos) == marker) {
            counter++;
          }
        }
        if (counter == 3) {
          return true;
        }
      }
    }
    return false;
  }

  /**
   * @brief Searches for potential mills to be closed
   *
   * @param f The board
   * @param marker The marker to search potential mills for
   * @return std::pair<Mill, Position> A potential mill and the free position of it. The position is invalid, if no potential mill was found
   */
  static std::pair<Mill, Position> check_potential_mills(Field& f, char marker) {
    // Returns a mill that can be completed and the free position to complete it

    for (const auto& mill : valid_mills_array) {
      int user_in_question_stones = 0;
      int other_user_stones = 0;
      Position free_pos = Position();

      // Check every possible mill for completable mills
      for (Position pos : mill) {
        char field_marker = f.get_field_marker_at_position(pos);
        if (field_marker == marker) {
          user_in_question_stones++;
        } else if (field_marker != f.EMPTY_FIELD) {
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

  /**
   * @brief Checks whether the given position is part of the given mill or not
   *
   * @param target_pos Position to check
   * @param mill Mill to check
   * @return true Position is part of the mill
   * @return false POsition is not part of the mill
   */
  static bool pos_is_part_of_mill(Position& target_pos, const Mill& mill) {
    for (auto pos : mill) {
      if (pos == target_pos)
        return true;
    }
    return false;
  }

  /**
   * @brief Get all removable stones of this marker
   *
   * @param field The gameboard
   * @param marker The marker referring to the player's stones
   * @return std::vector<Position>
   */
  static std::vector<Position> get_removable_stones(Field& field, char marker) {

    // Get positions of players stones
    std::vector<Position> players_stones = field.get_all_players_stones(marker);
    std::set<Position> stones_in_mills;

    // Get all potential mills
    std::vector<Mill> semi_active_mills;
    for (auto& mill : valid_mills_array) {
      for (auto& pos : players_stones) {
        if (pos_is_part_of_mill(pos, mill)) {
          semi_active_mills.push_back(mill);
        }
      }
    }

    // Check if every position in this mill is occupied by this player
    // If yes, cache these positions
    for (auto& mill : semi_active_mills) {
      short counter = 0;
      for (auto& mill_pos : mill) {
        for (auto& player_pos : players_stones) {
          if (mill_pos == player_pos) {
            counter++;
          }
        }
      }
      if (counter == 3) {
        // Completed mill, cache positions
        for (auto& pos : mill) {
          stones_in_mills.insert(pos);
        }
      }
    }

    // If set has all of player's positions, every stone is within an active mill
    bool all_in_mill = players_stones.size() == stones_in_mills.size();

    // If every stone is within a mill, every stone can be removed
    if (all_in_mill) {
      return players_stones;
    }

    // Return all positions which are not included within completed mills
    for (auto& pos : stones_in_mills) {
      players_stones.erase(
          std::remove(players_stones.begin(), players_stones.end(), pos),
          players_stones.end());
    }
    return players_stones;
  }
};

#endif
