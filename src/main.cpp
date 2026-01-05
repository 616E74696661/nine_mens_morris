#include "mills.hpp"
#include "settings.cpp"
#include <array>
#include <iostream>
#include <vector>

void handle_mill();
void handle_opening();
void handle_midgame();
void handle_endgame();

Settings setting;
std::vector<user*> players = setting.setup();

Mills m;

user* active_player = players.at(0);

int main() {
  std::cout << game_text::WELCOME << std::endl;

  int i = 0;
  // Gameloop
  bool run = true;
  while (run) {

    m.field_output();

    switch (m.get_current_phase()) {
    case Mills::OPENING:
      handle_opening();
      break;
    case Mills::MIDGAME:
      handle_midgame();
      break;
    case Mills::ENDGAME:
      handle_endgame();
      break;
    default:
      break;
    }
    // check for mills
    handle_mill(&m, &pos, active_player->marker);
    // switch active player
    active_player =
        players.at((++i) % 2);
  }
}

void handle_opening() {
  // Set stone position
  Position pos = active_player->place_marker();

  bool success = m.player_set_stone(active_player->marker, &pos);
  if (!success) {
    // Retry placing marker
    handle_opening();
    return;
  }

  if (m.all_stones_set())
    m.next_phase();
}

void handle_midgame() {
}

void handle_endgame() {
}

void handle_mill(Mills* m, Position* pos, char marker) {
  std::vector<std::array<std::pair<int, int>, 3>> mills =
      pos->getPossibleMills();
  for (const auto& mill : mills) {
    if (m->check_mill(mill, marker)) {
      std::cout << "Mill formed!" << std::endl;
      // TODO: handle removing opponent's piece
    }
  }
}