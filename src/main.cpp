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
User& get_inactive_player(User& active_user, const std::vector<User*>& players);
void game_lost(User& loser);
Position handle_opening();
// Position handle_midgame();
Position handle_endgame();

Settings setting;
Field f;
Position pos;
std::vector<User*> players;
User* active_user = nullptr;
int MAX_NUM_STONES = 9;

/**
 * @brief Gamestart and gameloop
 *
 * @return int
 */
int main() {

  init();

  // ensure active player is set
  if (active_user == nullptr) {
    throw std::logic_error("No active player set");
  }

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

/**
 * @brief Initialize game and players, set active player
 *
 */
void init() {
  players = setting.setup();
  if (players.empty())
    throw std::logic_error("No players");
  active_user = players.at(0);
  std::cout << game_text::WELCOME << std::endl;
}

/**
 * @brief Checks whether the game is over or not
 *
 * @param active_user
 */
void check_game_over(User& active_user) {

  if (!f.available_to_move(active_user)) {
    game_lost(active_user);
  }

  if (active_user.get_stones_on_board() < 3) {
    game_lost(active_user);
  }
}

/**
 * @brief Placing, moving and removing stones
 *
 * @return Position
 */
Position game() {

  // check if user is a player
  if (!active_user->is_bot()) {
    f.field_output();
    std::cout << "It's your turn, " << active_user->name << " (" << active_user->marker << ")" << std::endl;
  }

  std::string output;
  Position new_pos;

  int placed_stones = active_user->get_stones_set();

  std::cout << ">>> Set: " << active_user->get_stones_set() << " Removed: " << active_user->get_stones_removed() << " Board: " << active_user->get_stones_on_board() << std::endl;

  if (placed_stones < MAX_NUM_STONES) {
    // Opening phase: place stones
    new_pos = active_user->place_marker(f);
    output = active_user->name + " placed a stone on Position y: " + std::to_string(new_pos.y) + ", x: " + std::to_string(new_pos.x);
  } else if (placed_stones == MAX_NUM_STONES) {
    // Midgame / Endgame phase: move stones

    // Check game over
    check_game_over(*active_user);

    // Move stone
    std::pair<Position, Position> pos_pair;
    bool three_stones_left = active_user->get_stones_on_board() == 3;
    pos_pair = active_user->move_marker(f, three_stones_left);

    output = active_user->name + " moved a stone from Position y: " + std::to_string(pos_pair.first.y) + ", x: " + std::to_string(pos_pair.first.x) +
             " to Position y: " + std::to_string(pos_pair.second.y) + ", x: " + std::to_string(pos_pair.second.x);
    new_pos = pos_pair.second;

  } else {
    throw std::range_error("FATAL ERROR. PLAYER PLAYED MORE THAN 9 STONES.");
  }

  std::cout << ">>> Set: " << active_user->get_stones_set() << " Removed: " << active_user->get_stones_removed() << " Board: " << active_user->get_stones_on_board() << std::endl;

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
void check_for_closed_mill(Field& field, Position& pos, User& active_user) {
  // Check for mills
  bool mill_closed = Mills::check_mill(pos, active_user.marker, field);
  // If mill is closed, remove opponent's stone
  if (mill_closed) {
    Position removed_stone = active_user.remove_opponent_marker(field);

    get_inactive_player(active_user, players).remove_stone();

    std::cout << active_user.name << " formed a mill -> removed stone of " << get_inactive_player(active_user, players).name
              << " on Position y: " << removed_stone.y << "x: " << removed_stone.x << std::endl;
  } else
    std::cout << std::endl;
}

/**
 * @brief Get the other player object
 *
 * @param active_user
 * @param players
 * @return User&
 */
User& get_inactive_player(User& active_user, const std::vector<User*>& players) {
  if (players.size() != 2) {
    throw std::logic_error("This function only works with 2 players");
  }
  return *(
      (players[0] == &active_user) ? players[1] : players[0]);
}

/**
 * @brief Handle gameover
 *
 * @param loser Player who lost the game
 */
void game_lost(User& loser) {
  User& other = get_inactive_player(*active_user, players);
  std::cout << active_user->name << " has lost!" << std::endl;
  std::cout << other.name << " wins the game!" << std::endl;
  std::exit(0);
}