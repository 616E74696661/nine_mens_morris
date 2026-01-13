#include "user.hpp"
#include "field.hpp" // full definition of Field is needed
#include <vector>

// "User::" so we can use the user variables

bool User::able_to_make_legal_move(Field& f) {
  std::vector<Position> own_stones;

  // collect all own stone positions
  for (unsigned int y = 1; y <= 7; ++y) {
    for (unsigned int x = 1; x <= 7; ++x) {
      Position pos(x, y);
      if (!pos.is_valid())
        continue;

      if (f.get_field_marker_at_position(pos) == this->marker) {
        own_stones.push_back(pos);
      }
    }
  }

  // no legal moves bc 0 stones, should not be possible to happen
  if (own_stones.empty())
    return false;

  // endgame: flying allowed
  if (own_stones.size() == 3) {
    // if there is at least one empty position anywhere -> can move
    for (unsigned int y = 1; y <= 7; ++y) {
      for (unsigned int x = 1; x <= 7; ++x) {
        Position pos(x, y);
        if (!pos.is_valid())
          continue;

        if (f.get_field_marker_at_position(pos) == '#') {
          return true;
        }
      }
    }
    return false;
  }

  // midgame: normal moves (must have empty neighbour)
  for (Position& stone_pos : own_stones) {
    std::vector<Position*> neighbours = stone_pos.get_neighbours();
    for (Position* neighbour : neighbours) {
      if (f.get_field_marker_at_position(*neighbour) == '#') {
        return true;
      }
    }
  }

  // No legal moves found
  return false;
}