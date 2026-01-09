#include "Mills.hpp"
#include "settings.cpp"
#include <array>
#include <iostream>
#include <vector>

void handle_mill(Mills*, Position*, char);
Position handle_opening();
Position handle_midgame();
Position handle_endgame();

Settings setting;
std::vector<user*> players = setting.setup();

Mills m;
Position pos;

user* active_player = players.at(0);

int main() {
  std::cout << game_text::WELCOME << std::endl;

  int i = 0;
  // Gameloop
  bool run = true;
  while (run) {
    switch (m.get_current_phase()) {
    case Mills::OPENING:
      pos = handle_opening();
      break;
    case Mills::MIDGAME:
      pos = handle_midgame();
      break;
    case Mills::ENDGAME:
      pos = handle_endgame();
      break;
    default:
      return 0;
      break;
    }

    // check for mills
    handle_mill(&m, &pos, active_player->marker);
    // switch active player
    active_player =
        players.at((++i) % 2);
  }
}

Position handle_opening() {
  m.field_output();
  // Set stone position
  Position pos = active_player->place_marker(&m);

  std::cout << pos.y << " " << pos.x << std::endl;
  bool success = m.player_set_stone(active_player->marker, &pos);
  if (!success) {
    // Retry placing marker
    return handle_opening();
  }

  if (m.all_stones_set())
    m.next_phase();

  return pos;
}

// works (but is not pretty)
// need to test this more and add error handling
Position handle_midgame() {
  m.field_output();
  // TODO: check if player can make a move (if not -> instantly lost)
  std::pair<Position, Position> move_marker = active_player->move_marker();
  Position old_pos = move_marker.first;
  Position new_pos = move_marker.second;

  std::cout << old_pos.y << " " << old_pos.x << std::endl;

  bool success = m.player_remove_stone(active_player->marker, &old_pos);
  if (!success) {
    // Retry placing marker
    return handle_midgame();
  }

  success = m.player_set_stone(active_player->marker, &new_pos);
  if (!success) {
    m.player_set_stone(active_player->marker, &old_pos);
    // Retry placing marker
    return handle_midgame();
  }
  return move_marker.second;
}

Position handle_endgame() {
  m.field_output();
  // Position pos = active_player->move_marker();
  return Position();
}

void handle_mill(Mills* m, Position* pos, char marker) {
  std::vector<std::array<std::pair<int, int>, 3>> mills =
      pos->get_possible_mills();
  for (const auto& mill : mills) {
    if (m->check_mill(mill, marker)) {
      active_player->remove_opponent_marker(m);
    }
  }
}