#ifndef MILLS_CLASS_HPP
#define MILLS_CLASS_HPP

#include "constants/error_messages.hpp"
#include "constants/game_text.hpp"
#include <iostream>

class Mills {
public:
  Mills() { start_game(); }

  void field_output() {
    for (int i = 0; i < Y; i++) {
      if (i % 2 == 0 && i != Y - 1)
        std::cout << 7 - (i / 2) << "  ";
      else
        std::cout << "   ";
      std::cout << field_template[i] << std::endl;
    }
  }

  int player_set_stone(int player, int pos_y, int pos_x) {
    pos_y = get_y_pos(pos_y);
    pos_x = get_x_pos(pos_x);
    printf("y: %i, x: %i\n", pos_y, pos_x);
    if (!(pos_y < Y && pos_x < X) || field_template[pos_y][pos_x] != '#') {
      std::cout << game_text::INVALID_INPUT << std::endl;
      return 0;
    }
    field_template[pos_y][pos_x] = player == 0 ? 'O' : 'X';
    std::cout << "successfully set :3" << std::endl;
    return 1;
  }

private:
  static const int X = 26;
  static const int Y = 15;
  static const int STONES = 24;
  static const char T = '#';

  // int mills_field[STONES];

  char field_template[Y][X] = {
      "#-----------#-----------#", "|           |           |",
      "|   #-------#-------#   |", "|   |       |       |   |",
      "|   |   #---#---#   |   |", "|   |   |       |   |   |",
      "#---#---#       #---#---#", "|   |   |       |   |   |",
      "|   |   #---#---#   |   |", "|   |       |       |   |",
      "|   #-------#-------#   |", "|           |           |",
      "#-----------#-----------#", "",
      "1   2   3   4   5   6   7"};

  // irrelevant when saving as [7][7] arr
  int get_y_pos(int pos_y) { return ((7 - (pos_y)) * 2); }

  // irrelevant when saving as [7][7] arr
  int get_x_pos(int pos_x) { return ((pos_x - 1) * 4); }

  int start_game() {
    std::cout << std::endl << game_text::START << std::endl << std::endl;
    // settings();
    return 1;
  }

  /*int settings() {
    std::cout << game_text::SETTINGS << std::endl << std::endl;
    return 1;
  }*/

  /*int create_field() {
    return 1;
  }*/

  // irrelevant when saving as [7][7] arr
  int calc_pos(int pos_y, int pos_x) {
    return get_y_pos(pos_y) + get_x_pos(pos_x);
  }
};
#endif