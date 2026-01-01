#include "mills.cpp"
#include "settings.cpp"
#include "constants/error_messages.hpp"

int main() {
  std::cout << game_text::WELCOME << std::endl;
  
  Settings setting;
  std::vector<user*> players = setting.setup();

  Mills m;
  m.field_output();

  user* active_player = players.at(0);

  for (int i = 0; i < 10; i++) {
    active_player->place_marker(&m);
    active_player = players.at((i + 1) % players.size()); // switch active player
    m.field_output();
  }
}