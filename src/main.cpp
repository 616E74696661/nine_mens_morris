#include "field.hpp"
#include "mill.hpp"
#include "settings.cpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

void init();
Position game();
void check_for_closed_mill(Field&, Position&, User&);
User& get_other_player(User& active_user, const std::vector<User*>& players);
void game_lost(User& loser);
Position handle_opening();
// Position handle_midgame();
Position handle_endgame();

Settings setting;
Field f;
Position pos;
std::vector<User*> players;
User* active_user = nullptr;

int main() {

  init();

  int iteration = 0;
  while (true) {

    // handle a turn
    pos = game();

    // check for closed mill
    check_for_closed_mill(f, pos, *active_user);

    // switch active player
    active_user = players.at((++iteration) % 2);
  }
}

void init() {
  players = setting.setup();
  if (players.empty())
    throw std::logic_error("No players");
  active_user = players.at(0);
  std::cout << game_text::WELCOME << std::endl;
}

Position game() {

  // ensure active player is set
  if (active_user == nullptr) {
    throw std::logic_error("No active player set");
  }

  // check if user is a player
  if (!active_user->is_bot()) {
    f.field_output();
    std::cout << "It's your turn, " << active_user->name << " (" << active_user->marker << ")" << std::endl;
  }

  std::string output;
  Position new_pos;

  int placed_stones = active_user->get_stones_set();
  int stones_left = placed_stones - active_user->get_stones_removed();

  if (placed_stones < 9) {
    // Opening phase: place stones
    new_pos = active_user->place_marker(f);
    output = active_user->name + " placed a stone on Position y: " + std::to_string(new_pos.y) + ", x: " + std::to_string(new_pos.x);
  } else if (placed_stones == 9) {
    // Midgame / Endgame phase: move stones

    /*
    if (!active_user->able_to_make_legal_move(f)) {
      // check if a player lost
      game_lost(*active_user);
    }
    */

    std::pair<Position, Position> pos_pair;
    bool three_stones_left = active_user->get_stones_on_board() == 3;
    pos_pair = active_user->move_marker(f, three_stones_left);

    output = active_user->name + " moved a stone from Position y: " + std::to_string(pos_pair.first.y) + ", x: " + std::to_string(pos_pair.first.x) +
             " to Position y: " + std::to_string(pos_pair.second.y) + ", x: " + std::to_string(pos_pair.second.x);
    new_pos = pos_pair.second;
  } else {
    throw std::range_error("FATAL ERROR. PLAYER PLAYED MORE THAN 9 STONES.");
  }

  // Helper::clear_console();
  std::cout << output << std::endl;
  return new_pos;
}

/**
 * @brief checks if a mill has been closed through the new stone
 *
 * @param f
 * @param pos position where a stone was placed
 */
void check_for_closed_mill(Field& f, Position& pos, User& active_user) {
  // Check for mills
  bool mill_closed = Mills::check_mill(pos, active_user.marker, f);
  // If mill is closed, remove opponent's stone
  if (mill_closed) {
    Position removed_stone = active_user.remove_opponent_marker(f, get_other_player(active_user, players));
    std::cout << active_user.name << " formed a mill -> removed stone of " << get_other_player(active_user, players).name
              << " on Position y: " << removed_stone.y << "x: " << removed_stone.x << std::endl;
  } else
    std::cout << std::endl;
}

User& get_other_player(User& active_user, const std::vector<User*>& players) {
  if (players.size() != 2) {
    throw std::logic_error("This function only works with 2 players");
  }
  return *(
      (players[0] == &active_user) ? players[1] : players[0]);
}

void game_lost(User& loser) {
  User& other = get_other_player(*active_user, players);
  std::cout << active_user->name << " has lost!" << std::endl;
  std::cout << other.name << " wins the game!" << std::endl;
  std::exit(0);
}