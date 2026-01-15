#include "field.hpp"
#include "mill.hpp"
#include "position.hpp"
#include "user.hpp"
#include <exception>
#include <float.h>
#include <iostream>
#include <utility>
#include <vector>

class BotUser : public User {
private:
  std::string name = "Bot";
  char opponent_marker = 'O';

  /**
   * @brief get a random stone position of opponent
   *
   * @param f Field
   * @return Position
   */
  Position get_random_opponent_position(Field& f) {
    Position random_pos;
    do
      random_pos = get_random_position();
    while (f.get_field_marker_at_position(random_pos) == this->marker ||
           f.get_field_marker_at_position(random_pos) == '#');
    return random_pos;
  }

  // Position move_stone_to_potential_mill(Field& f, char marker) {
  //   std::pair<Mill, Position> mill_and_pos = Mills::check_potential_mills(f, marker);
  //   if ()
  //     return Position(0, 0);
  // }

  Position select_stone_to_move(Field& f, Position* target_pos, Mill* mill, bool* jump_allowed) {

    if (jump_allowed) {
      // select stone which is not included within the potential mill
      for (auto pos : f.get_all_players_stones(marker)) {
        if (!Mills::pos_is_part_of_mill(*target_pos, *mill)) {
          return *target_pos;
        }
      }
    }

    // Search for adjacent stone to the target position
    std::vector<Position*> adj_positions = target_pos->get_adjacent_positions();
    for (auto pos : adj_positions) {
      if (!Mills::pos_is_part_of_mill(*pos, *mill) && f.get_field_marker_at_position(*pos) == marker) {
        // Position is not part of the mill and occupied by this bot
        return *pos;
      }
    }

    // Programm should not be able to get here
    throw "Reached unexpected point in code";
  }

  std::pair<Position, Position> get_move(Field& f, bool jump_allowed) {
    // try to complete own potential mill
    std::pair<Mill, Position> mill_and_pos = Mills::check_potential_mills(f, this->marker);
    Mill target_mill = mill_and_pos.first;
    Position target_pos = mill_and_pos.second;

    Position moveable_stone;
    if (target_pos.is_valid()) {
      moveable_stone = select_stone_to_move(f, &target_pos, &target_mill, &jump_allowed);
      f.validate_coordinates(moveable_stone, marker);
      return std::pair<Position, Position>(moveable_stone, target_pos);
    }

    // try to block opponents potential mill
    mill_and_pos = Mills::check_potential_mills(f, opponent_marker);
    target_mill = mill_and_pos.first;
    target_pos = mill_and_pos.second;

    if (target_pos.is_valid()) {
      moveable_stone = select_stone_to_move(f, &target_pos, &target_mill, &jump_allowed);
      f.validate_coordinates(moveable_stone, marker);
      return std::pair<Position, Position>(moveable_stone, target_pos);
    }

    if (jump_allowed) {
      // return random stone to jump to an empty position
      Position old_pos = f.get_all_players_stones(marker)[0];
      Position new_pos = f.get_all_players_stones('#')[0];
      return std::pair<Position, Position>(old_pos, new_pos);

    } else {
      // Search for stone with empty neighbour
      std::vector<Position> own_stones = f.get_all_players_stones(marker);
      for (auto pos : own_stones) {
        for (auto neighbour : pos.get_adjacent_positions()) {
          if (f.get_field_marker_at_position(*neighbour) == '#') {
            return std::pair<Position, Position>(pos, *neighbour);
          }
        }
      }
    }

    // Programm should not be able to get here
    throw "Reached unexpected point in code";
  }

public:
  BotUser(std::string name) : User(name) {}

  Position place_marker(Field& f, bool moved = false) override {
    std::pair<Mill, Position> mill_and_pos;
    Position pos;
    try {
      // block opponent's potential mill
      mill_and_pos = Mills::check_potential_mills(f, opponent_marker);
      pos = mill_and_pos.second;
      if (pos.is_valid() && f.player_set_stone(*this, pos, moved))
        return pos;

      // complete own potential mill
      mill_and_pos = Mills::check_potential_mills(f, this->marker);
      pos = mill_and_pos.second;
      if (pos.is_valid() && f.player_set_stone(*this, pos, moved))
        return pos;
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }

    // fallback: random placement
    while (true) {
      try {
        pos = get_random_position();
        if (f.player_set_stone(*this, pos, moved))
          return pos;
      } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
      }
    }

    std::cout << "Failed to place marker" << std::endl;
    return Position();
  }

  std::pair<Position, Position> move_marker(Field& f, bool three_stones_left) override {

    // Get move to make
    std::pair<Position, Position> move = get_move(f, three_stones_left);
    Position old_pos = move.first;
    Position new_pos = move.second;

    // Update board
    f.player_remove_stone(*this, old_pos);
    f.player_set_stone(*this, new_pos);

    return move;

    /*
    try {
      // try to complete own potential mill
      std::pair<Mill, Position> mill_and_pos = Mills::check_potential_mills(f, this->marker);
      Position target_pos = mill_and_pos.second;
      if (target_pos.is_valid()) {
        if (!three_stones_left) {
          std::vector<Position*> neighbours = target_pos.get_adjacent_positions();
          for (auto neighbour : neighbours) {
            if (f.get_field_marker_at_position(*neighbour) == this->marker) {
              // remove stone from current position
              if (f.player_remove_stone(*this, *neighbour)) {
                // try to place at target position
                if (f.player_set_stone(*this, target_pos, true))
                  return std::make_pair(*neighbour, target_pos);
                // rollback if placement failed
                f.player_set_stone(*this, *neighbour, true);
              }
            }
          }
        } else {
          // TODO
        }
      }
      // fallback: random move
      int retries = 10;
      while (retries-- > 0) {
        Position from_pos = get_random_position();
        Position to_pos = get_random_position();
        // check if from_pos is our stone
        if (f.get_field_marker_at_position(from_pos) != this->marker)
          continue;
        // check adjacency constraint (unless we can jump with 3 stones)
        if (!three_stones_left && !from_pos.is_adjacent_position(from_pos, to_pos))
          continue;
        // try the move
        if (f.player_remove_stone(*this, from_pos))
          if (f.player_set_stone(*this, to_pos, true))
            return std::make_pair(from_pos, to_pos);
        // rollback
        f.player_set_stone(*this, from_pos, true);
      }
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }
    return std::pair<Position, Position>(Position(), Position());
  */
  }

  Position remove_opponent_marker(Field& f, User& other) override {
    Position removable_stone;

    while (true) {
      try {
        // try to block opponent's potential mill
        std::pair<Mill, Position> mill_and_pos = Mills::check_potential_mills(f, this->marker);
        Mill mill = mill_and_pos.first;
        Position pos = mill_and_pos.second;

        if (pos.is_valid())
          for (std::size_t i = 0; i < mill.size(); ++i) {
            if (mill[i] == pos)
              continue;

            char field_marker = f.get_field_marker_at_position(mill[i]);
            if (field_marker != this->marker && field_marker != '#') {
              removable_stone = mill[i];
              break;
            }
          }

        // fallback: random opponent stone
        if (!removable_stone.is_valid())
          removable_stone = get_random_opponent_position(f);

        if (f.opponent_remove_stone(this->marker, removable_stone, other))
          break;
      } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
      }
    }
    return removable_stone;
  }

  bool is_bot() override {
    return true;
  }
};