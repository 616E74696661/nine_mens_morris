#include "mills.cpp"
#include "settings.cpp"
#include "constants/error_messages.hpp"


void handle_mills(Mills* m, Position* pos, char marker) {
  std::vector<std::array<std::pair<int, int>, 3>> mills = pos->getPossibleMills();
  for (const auto &mill : mills) {
    if (m->check_mill(mill, marker)) {
      std::cout << "Mill formed!" << std::endl;
      //TODO: handle removing opponent's piece
    }
  }
}


int main() {
  std::cout << game_text::WELCOME << std::endl;
  
  Settings setting;
  std::vector<user*> players = setting.setup();

  Mills m;
  m.field_output();

  user* active_player = players.at(0);

  for (int i = 0; i < 10; i++) {
    // Select position
    Position pos = active_player->place_marker();
    // Place marker
    m.player_set_stone(active_player->marker, &pos);
    // Check for mills
    handle_mills(&m, &pos, active_player->marker);
    active_player = players.at((i + 1) % players.size()); // switch active player
    m.field_output();
  }

  // Gameloop
  bool run = true;
  while (run) {

  }

}