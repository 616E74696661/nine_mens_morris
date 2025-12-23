#include "constants/game_text.hpp"
#include <iostream>

class Mills {
public:
  Mills() { start_game(); }

  void ouput_field() {
    int pos_count = 0;
    for (int i = 0; i < 13; i++) {
      // only every 2 rows are numerated
      if (i % 2 == 0)
        std::cout << 7 - (i / 2) << "   ";
      else
        std::cout << "    ";
      for (int j = 0; j < 26; j++) {
        if (FIELD_TEMPLATE[i][j] == '#') {
          // get field content
          int pos_y = std::get<0>(VALID_POSITIONS[pos_count]);
          int pos_x = std::get<1>(VALID_POSITIONS[pos_count++]);
          std::cout << mills_field[pos_y][pos_x];
        } else
          std::cout << FIELD_TEMPLATE[i][j];
      }
      std::cout << std::endl;
    }
    std::cout << "\n    " << "1   2   3   4   5   6   7" << std::endl;
  }

  int set_stone(int player, int pos_x, int pos_y) {
    pos_x = calc_x_pos(pos_x);
    pos_y = calc_y_pos(pos_y);

    // return if invalid input
    if (!valid_input(pos_x, pos_y)) {
      std::cout << game_text::INVALID_INPUT << std::endl;
      return 0;
    }

    mills_field[pos_y][pos_x] = player == 0 ? 'O' : 'X';

    std::cout << "successfully set stone" << std::endl;
    return 1;
  }

private:
  static const int STONES = 24;
  static const char T = ' ';

  const std::pair<int, int> VALID_POSITIONS[STONES] = {
      {0, 0}, {0, 3}, {0, 6}, {1, 1}, {1, 3}, {1, 5}, {2, 2}, {2, 3},
      {2, 4}, {3, 0}, {3, 1}, {3, 2}, {3, 4}, {3, 5}, {3, 6}, {4, 2},
      {4, 3}, {4, 4}, {5, 1}, {5, 3}, {5, 5}, {6, 0}, {6, 3}, {6, 6}};

  const char FIELD_TEMPLATE[15][26] = {
      "#-----------#-----------#", "|           |           |",
      "|   #-------#-------#   |", "|   |       |       |   |",
      "|   |   #---#---#   |   |", "|   |   |       |   |   |",
      "#---#---#       #---#---#", "|   |   |       |   |   |",
      "|   |   #---#---#   |   |", "|   |       |       |   |",
      "|   #-------#-------#   |", "|           |           |",
      "#-----------#-----------#",
  };

  char mills_field[7][7] = {
      {T, '\0', '\0', T, '\0', '\0', T}, {'\0', T, '\0', T, '\0', T, '\0'},
      {'\0', '\0', T, T, T, '\0', '\0'}, {T, T, T, '\0', T, T, T},
      {'\0', '\0', T, T, T, '\0', '\0'}, {'\0', T, '\0', T, '\0', T, '\0'},
      {T, '\0', '\0', T, '\0', '\0', T}};

  int calc_y_pos(int pos_y) { return (7 - (pos_y)); }

  int calc_x_pos(int pos_x) { return pos_x - 1; }

  int start_game() {
    std::cout << std::endl << game_text::START << std::endl << std::endl;
    return 1;
  }

  int valid_input(int pos_x, int pos_y) {
    return (pos_x < 7 && pos_y < 7 && mills_field[pos_y][pos_x] == T);
  }
};