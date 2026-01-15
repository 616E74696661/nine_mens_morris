#ifndef USER_CLASS_HPP
#define USER_CLASS_HPP

#include "constants/error_messages.hpp"
#include "position.hpp"
#include <string>

class Field; // forward declaration, else clangd error

static int num_players = 0;

class User {
private:
  int stones_set;
  int stones_removed; // stones_set minus player stones on the board
  void define_marker() {
    if (num_players == 1) {
      this->marker = 'O';
    } else if (num_players == 2) {
      this->marker = 'X';
    } else {
      throw std::logic_error(error_msg::TOO_MANY_PLAYERS);
    }
  }

protected:
  virtual ~User() = default;

public:
  char marker;
  std::string name;
  User(std::string name) : name(name) {
    num_players++;
    define_marker();
    stones_set = 0;
    stones_removed = 0;
  }

  bool able_to_make_legal_move(Field& f);

  int get_stones_on_board() {
    return stones_set - stones_removed;
  }

  int get_stones_set() {
    return stones_set;
  }

  int get_stones_removed() {
    return stones_removed;
  }

  void set_stone() {
    stones_set++;
  }

  void remove_stone() {
    stones_removed++;
  }

  bool lost(Field& f);

  virtual Position place_marker(Field& f) = 0;

  virtual std::pair<Position, Position> move_marker(Field& f, bool three_stones_left) = 0;

  virtual Position remove_opponent_marker(Field& f, User& other) = 0;

  virtual bool is_bot() {
    return false;
  }
};

#endif