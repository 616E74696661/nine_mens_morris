#ifndef MILLS_CLASS_HPP
#define MILLS_CLASS_HPP

#include "constants/error_messages.hpp"
#include "constants/game_text.hpp"
#include "position.hpp"
#include <iostream>

class Mills {
public:
  Mills() { }

  void field_output() {
    for (int i = 0; i < Y; i++) {
      if (i % 2 == 0 && i != Y - 1)
        std::cout << 7 - (i / 2) << "  ";
      else
        std::cout << "   ";
      std::cout << field_template[i] << std::endl;
    }
  }

  int player_set_stone(char marker, Position* pos) {
    int pos_y = get_y_pos(pos->y);
    int pos_x = get_x_pos(pos->x);

    if (field_template[pos_y][pos_x] != T) {
      throw error_msg::POSITION_OCCUPIED;
    }

    field_template[pos_y][pos_x] = marker;
    std::cout << "successfully set :3" << std::endl;
    return 1;
  }
  
  bool check_mill(std::array<std::pair<int, int>, 3> mill, char marker) {

    int counter = 0;

    for (const auto &pos : mill) {
      int pos_y = get_y_pos(pos.second);
      int pos_x = get_x_pos(pos.first);
      if (field_template[pos_y][pos_x] == marker) {
        counter++;
      }
    }

    return counter == 3;
  }

private:
  static const int X = 26;
  static const int Y = 15;
  static const int STONES = 24;
  static const char T = '#';

  // int mills_field[STONES];

  char field_template[Y][X] = {
      "#-----------#-----------#", 
      "|           |           |",
      "|   #-------#-------#   |", 
      "|   |       |       |   |",
      "|   |   #---#---#   |   |", 
      "|   |   |       |   |   |",
      "#---#---#       #---#---#", 
      "|   |   |       |   |   |",
      "|   |   #---#---#   |   |", 
      "|   |       |       |   |",
      "|   #-------#-------#   |", 
      "|           |           |",
      "#-----------#-----------#", "",
      "1   2   3   4   5   6   7"};

  int get_y_pos(int pos_y) { return ((7 - (pos_y)) * 2); }

  // irrelevant when saving as [7][7] arr
  int get_x_pos(int pos_x) { return ((pos_x - 1) * 4); }

  // irrelevant when saving as [7][7] arr
  int calc_pos(int pos_y, int pos_x) {
    return get_y_pos(pos_y) + get_x_pos(pos_x);
  }

};
#endif