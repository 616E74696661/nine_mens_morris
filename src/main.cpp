#include "field.hpp"
#include "mill.hpp"
#include "settings.cpp"
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
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
    BotUser* bot_player = dynamic_cast<BotUser*>(active_player);
    std::string output;
    Position new_pos;
    if (bot_player == nullptr) {
      f.field_output();
      std::cout << "It's your turn, " << active_player->name << "!" << std::endl;
    }
    unsigned int placed_stones = active_player->get_stones_set();
    if (placed_stones < 9) {
      new_pos = active_player->place_marker(f);
      output = active_player->name + " placed a stone on Position y: " + std::to_string(new_pos.y) + ", x: " + std::to_string(new_pos.x);
    } else if (placed_stones == 9) {

      if (active_player->able_to_make_legal_move(f)) {
        User& other = get_other_player(*active_player, players);
        std::cout << active_player->name << " has lost!" << std::endl;
        std::cout << other.name << " wins the game!" << std::endl;
        std::exit(0);
      }

      std::pair<Position, Position> pos_pair;
      if (active_player->get_stones_removed() == 3)
        pos_pair = active_player->move_marker(f, true);
      else
        pos_pair = active_player->move_marker(f);

      output = active_player->name + " moved a stone from Position y: " + std::to_string(pos_pair.first.y) + ", x: " + std::to_string(pos_pair.first.x) +
               " to Position y: " + std::to_string(pos_pair.second.y) + ", x: " + std::to_string(pos_pair.second.x);
      new_pos = pos_pair.second;
    } else
      throw std::range_error("FATAL ERROR. PLAYER PLAYED MORE THAN 9 STONES.");

    // Helper::clear_console();
    std::cout << output << std::endl;
    return new_pos;
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
    Position removed_stone = active_player.remove_opponent_marker(f, get_other_player(active_player, players));
    std::cout << active_player.name << " formed a mill -> removed stone of " << get_other_player(active_player, players).name
              << " on Position y: " << removed_stone.y << "x: " << removed_stone.x << std::endl;
  } else
    std::cout << std::endl;
}

User& get_other_player(User& active_player, const std::vector<User*>& players) {
  if (players.size() != 2) {
    throw std::logic_error("This function only works with 2 players");
  }
  return *(
      (players[0] == &active_player) ? players[1] : players[0]);
}
