#include "Field.hpp"
#include "mill.hpp"
#include "settings.cpp"
#include <iostream>
#include <vector>

void handle_mill(Field*, Position*, char);
Position handle_opening();
Position handle_midgame();
Position handle_endgame();

Settings setting;
std::vector<user*> players = setting.setup();

Field f;
Position pos;

user* active_player = players.at(0);

int main() {
  std::cout << game_text::WELCOME << std::endl;

  int i = 0;
  // Gameloop
  bool run = true;
  while (run) {
    switch (f.get_current_phase()) {
    case Field::OPENING:
      pos = handle_opening();
      break;
    case Field::MIDGAME:
      pos = handle_midgame();
      break;
    case Field::ENDGAME:
      pos = handle_endgame();
      break;
    default:
      return 0;
      break;
    }

    // check for mills
    handle_mill(&f, &pos, active_player->marker);
    // switch active player
    active_player =
        players.at((++i) % 2);
  }
}

Position handle_opening() {
  f.field_output();
  // Set stone position
  Position pos = active_player->place_marker(&f);

  std::cout << pos.y << " " << pos.x << std::endl;
  bool success = f.player_set_stone(active_player->marker, &pos);
  if (!success) {
    // Retry placing marker
    return handle_opening();
  }

  if (f.all_stones_set())
    f.next_phase();

  return pos;
}

// works (but is not pretty)
// need to test this more and add error handling
Position handle_midgame() {
  f.field_output();
  // TODO: check if player can make a move (if not -> instantly lost)
  std::pair<Position, Position> move_marker = active_player->move_marker();
  Position old_pos = move_marker.first;
  Position new_pos = move_marker.second;

  std::cout << old_pos.y << " " << old_pos.x << " -> " << new_pos.y << " " << new_pos.x << std::endl;

  bool success = f.player_move_stone(active_player->marker, &old_pos, &new_pos);
  if (!success) {
    // Retry placing marker
    return handle_midgame();
  }
  return move_marker.second;
}

Position handle_endgame() {
  f.field_output();
  // Position pos = active_player->move_marker();
  return Position();
}

void handle_mill(Field* f, Position* pos, char marker) {
  // Check for mills
  bool mill_formed = Mills::check_mill(*pos, marker, f);
  // If mill formed, remove opponent's marker
  if (mill_formed) {
    active_player->remove_opponent_marker(f);
  }
}