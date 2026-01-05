#include "constants/error_messages.hpp"
#include "mills.cpp"
#include "settings.cpp"

void handle_mills(Mills* m, Position* pos, char marker) {
  std::vector<std::array<std::pair<int, int>, 3>> mills =
      pos->getPossibleMills();
  for (const auto& mill : mills) {
    if (m->check_mill(mill, marker)) {
      std::cout << "Mill formed!" << std::endl;
      // TODO: handle removing opponent's piece
    }
  }
}

int main() {
  std::cout << game_text::WELCOME << std::endl;

  Settings setting;
  std::vector<user*> players = setting.setup();

  Mills m;

  user* active_player = players.at(0);
  int i = 0;
  // Gameloop
  bool run = true;
  while (run) {

    m.field_output();
    // Select position
    Position pos = active_player->place_marker();

    switch (m.get_current_phase()) {
    case Mills::OPENING:
      // invalid stone placed
      if (!m.player_set_stone(active_player->marker, &pos))
        continue;
      if (m.all_stones_set())
        m.next_phase();
      break;
    case Mills::MIDGAME:
      break;
    case Mills::ENDGAME:
      break;
    default:
      break;
    }
    // check for mills
    handle_mills(&m, &pos, active_player->marker);
    // switch active player
    active_player =
        players.at((++i) % 2);
  }
}