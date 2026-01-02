#include "mills.cpp"
#include "settings.cpp"
#include "constants/error_messages.hpp"


void handle_mills(Mills* m, Position* pos) {
  std::vector<std::array<std::pair<int, int>, 3>> mills = pos->getPossibleMills();
  for (const auto &mill : mills) {
    //m->check_mill(mill);
    std::cout<< mill[0].first << "," << mill[0].second << " - "
             << mill[1].first << "," << mill[1].second << " - "
             << mill[2].first << "," << mill[2].second << std::endl;  
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
    handle_mills(&m, &pos);
    active_player = players.at((i + 1) % players.size()); // switch active player
    m.field_output();
  }

  // Gameloop
  bool run = true;
  while (run) {

  }

}