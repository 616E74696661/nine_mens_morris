#ifndef USER_CLASS_HPP
#define USER_CLASS_HPP

#include "constants/error_messages.hpp"
#include "position.hpp"
#include <stdexcept>
#include <string>

class Field; // forward declaration, else clangd error

static unsigned int num_players = 0;

class User {
private:
  unsigned int stones_set;
  unsigned int stones_removed; // stones_set minus player stones on the board
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
  }

  unsigned int get_stones_set() {
    return stones_set;
  }

  unsigned int get_stones_removed() {
    return stones_removed;
  }

  void set_stone() {
    stones_set++;
  }

  void remove_stone() {
    stones_removed++;
  }

  virtual Position place_marker(Field& f) = 0;

  virtual std::pair<Position, Position> move_marker(Field& f, bool three_stones_left = false) = 0;

  virtual Position remove_opponent_marker(Field& f, User& other) = 0;
};

#endif