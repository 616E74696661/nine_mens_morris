#ifndef USER_CLASS_HPP
#define USER_CLASS_HPP

#include "Mills.hpp"
#include "constants/error_messages.hpp"
#include "position.hpp"
#include <string>

static int num_players = 0;

class user {
private:
  void define_marker() {
    if (num_players == 1) {
      this->marker = 'O';
    } else if (num_players == 2) {
      this->marker = 'X';
    } else {
      throw error_msg::TOO_MANY_PLAYERS;
    }
  }

protected:
  virtual ~user() = default;

public:
  char marker;
  std::string name;
  user(std::string name) : name(name) {
    num_players++;
    define_marker();
  }
  virtual Position place_marker(Mills* m) {
    return Position();
  }

  virtual std::pair<Position, Position> move_marker() {
    return std::make_pair(Position(), Position());
  }

  virtual void remove_opponent_marker(Mills* m) {
  }
};

#endif