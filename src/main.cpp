#include "field.hpp"
#include "mill.hpp"
#include "settings.cpp"
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>

void init();
Position game();
void check_for_closed_mill(Field&, Position&, User&);
User& get_other_player(User& active_player, const std::vector<User*>& players);
Position handle_opening();
// Position handle_midgame();
Position handle_endgame();

Settings setting;
Field f;
Position pos;
std::vector<User*> players;
User* active_player = nullptr;

int main() {
  init();
  unsigned int i = 0;
  while (true) {
    pos = game();
    check_for_closed_mill(f, pos, *active_player);
    // switch active player
    active_player =
        players.at((++i) % 2);
  }
}

void init() {
  players = setting.setup();
  if (players.empty())
    throw std::logic_error("No players");
  active_player = players.at(0);
  std::cout << game_text::WELCOME << std::endl;
}

Position game() {
  if (active_player != nullptr) {
    f.field_output();
    std::cout << "It's your turn, " << active_player->name << "!" << std::endl;
    unsigned int stones = active_player->get_stones_set();
    if (stones < 9) {
      std::cout << "Select stone position:" << std::endl;
      return active_player->place_marker(f);
    } else if (stones == 9) {
      std::cout << "Select stone you want to move:" << std::endl;
      if (active_player->get_stones_removed() == 3)
        return active_player->move_marker(f, true);
      else
        return active_player->move_marker(f);
    } else
      throw std::range_error("FATAL ERROR. PLAYER PLAYED MORE THAN 9 STONES.");
  }
  throw std::logic_error("");
}

/**
 * @brief checks if a mill has been closed through the new stone
 *
 * @param f
 * @param pos position where a stone was placed
 */
void check_for_closed_mill(Field& f, Position& pos, User& active_player) {
  // Check for mills
  bool mill_closed = Mills::check_mill(pos, active_player.marker, f);
  // If mill is closed, remove opponent's stone
  if (mill_closed) {
    active_player.remove_opponent_marker(f, get_other_player(active_player, players));
  }
}

User& get_other_player(User& active_player, const std::vector<User*>& players) {
  if (players.size() != 2) {
    throw std::logic_error("This function only works with 2 players");
  }
  return *(
      (players[0] == &active_player) ? players[1] : players[0]);
}
