#ifndef USER_CLASS_HPP
#define USER_CLASS_HPP

#include "constants/error_messages.hpp"
#include "position.hpp"
#include <string>

class Field; // forward declaration, else clangd error

static int num_players = 0;

class User {
private:
  int stones_set;     // number of stones set during the opening phase
  int stones_removed; // number of own stones removed by the opponent

  /**
   * @brief Defines the player's marker and the opponent's marker
   *
   */
  void define_marker() {
    if (num_players == 1) {
      this->marker = 'O';
      this->opponent_marker = 'X';
    } else if (num_players == 2) {
      this->marker = 'X';
      this->opponent_marker = 'O';
    } else {
      throw std::logic_error(error_msg::TOO_MANY_PLAYERS);
    }
  }

protected:
  /**
   * @brief Destroy the User object
   *  Important for inheritance
   */
  virtual ~User() = default;

public:
  char marker;          // Player's marker
  char opponent_marker; // Opponent's marker
  std::string name;     // Player's name

  /**
   * @brief Construct a new User object
   *
   * @param name Player's displayed name
   */
  User(std::string name) : name(name) {
    num_players++;
    define_marker();
    stones_set = 0;
    stones_removed = 0;
  }

  /**
   * @brief Get the number of stones currently on board
   *
   * @return int
   */
  int get_stones_on_board() {
    return stones_set - stones_removed;
  }

  /**
   * @brief Get the number of stones set by the player
   *
   * @return int
   */
  int get_stones_set() {
    return stones_set;
  }

  /**
   * @brief Get the number of this player's stones removed by the opponent
   *
   * @return int
   */
  int get_stones_removed() {
    return stones_removed;
  }

  /**
   * @brief Increase the number of stones set by the player by 1
   *
   */
  void set_stone() {
    stones_set++;
  }

  /**
   * @brief Increase the number of this player's stones removed by the opponent
   *
   */
  void remove_stone() {
    stones_removed++;
  }

  /**
   * @brief Abstract method to handle the placement of stones in the opening phase. To be implemented by inhereting classes
   *
   * @param f The field with the current gamestate
   * @return Position The position where the marker has been placed
   */
  virtual Position place_marker(Field& f) = 0;

  /**
   * @brief Abstract method to handle the movement of stones in the middle and end phase. To be implemented by inhereting classes
   *
   * @param f The field with the current gamestate
   * @param three_stones_left A boolean which decides whether a player is able to jump with it's stones
   * @return std::pair<Position, Position> The old position where the marker has been and the new position where it has been moved to
   */
  virtual std::pair<Position, Position> move_marker(Field& f, bool three_stones_left) = 0;

  /**
   * @brief Abstract method to handle the removing of an opponent's stone after successfully compleing a mill. To be implemented by inhereting classes
   *
   * @param f The field with the current gamestate
   * @return Position The position of the marker which has been removed
   */
  virtual Position remove_opponent_marker(Field& f) = 0;

  /**
   * @brief Returns a boolean which says whether the player is a bot or not
   *
   * @return true if player is a bot
   * @return false if player is a human player
   */
  virtual bool is_bot() {
    return false;
  }
};

#endif