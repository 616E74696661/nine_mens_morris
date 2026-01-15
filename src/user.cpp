#include "field.hpp" // full definition of Field is needed
#include <vector>

// "User::" so we can use the user variables

bool able_to_make_legal_move(Field& f) {
  std::vector<Position> own_stones;

  // Iteriere nur über die vordefinierten gültigen Positionen
  for (const auto& valid_pos : valid_positions) {
    Position pos(valid_pos.first, valid_pos.second);

    try {
      if (f.get_field_marker_at_position(pos) == this->marker) {
        own_stones.push_back(pos);
      }
    } catch (const std::exception& e) {
      // Sollte nicht passieren, da valid_positions nur gültige enthält
      continue;
    }
  }

  if (own_stones.empty())
    return false;

  // Endgame: flying allowed
  if (own_stones.size() == 3) {
    for (const auto& valid_pos : valid_positions) {
      Position pos(valid_pos.first, valid_pos.second);
      try {
        if (f.get_field_marker_at_position(pos) == '#') {
          return true;
        }
      } catch (const std::exception& e) {
        continue;
      }
    }
    return false;
  }

  // Midgame: check neighbours
  for (Position& stone_pos : own_stones) {
    std::vector<Position*> neighbours = stone_pos.get_adjacent_positions();
    for (Position* neighbour : neighbours) {
      try {
        if (f.get_field_marker_at_position(*neighbour) == '#') {
          return true;
        }
      } catch (const std::exception& e) {
        // Neighbour might be invalid, skip
        continue;
      }
    }
  }

  return false;
}