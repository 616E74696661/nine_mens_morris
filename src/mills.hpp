#ifndef MILLS_CLASS_HPP
#define MILLS_CLASS_HPP

#include "constants/error_messages.hpp"
#include "position.hpp"
#include <iostream>
#include <thread>

#ifdef _WIN32
const std::string OS = "windows";
#else
const std::string OS = "other"; // macos/linux
#endif

class Mills {
public:
  Mills() {
    current_phase = OPENING;
    stones_set = 0;
  }

  enum GamePhase {
    OPENING = 0,
    MIDGAME = 1,
    ENDGAME = 2
  };

  void field_output() {

    for (int i = 0; i < Y; i++) {
      if (i % 2 == 0 && i != Y - 1)
        std::cout << 7 - (i / 2) << "  ";
      else
        std::cout << "   ";
      std::cout << field_template[i] << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds((long long)MS));
    }
  }

  void clear_console() {
    for (int i = 0; i < 30; i++) {
      std::cout << "\n";
      std::this_thread::sleep_for(std::chrono::milliseconds((long long)MS));
    }
    if (OS == "windows")
      system("CLS");
    else // for linux and macos
      system("clear");
  }

  void stone_set() {
    stones_set++;
  }

  int all_stones_set() {
    return stones_set == STONES;
  }

  GamePhase get_current_phase() {
    return current_phase;
  }

  void next_phase() {
    if (current_phase < ENDGAME)
      current_phase = static_cast<GamePhase>(static_cast<int>(current_phase) + 1);
    std::cout << "NEXT GAMEPHASE" << std::endl;
  }

  bool player_set_stone(char marker, Position* pos) {
    int pos_y = get_y_pos(pos->y);
    int pos_x = get_x_pos(pos->x);

    clear_console();

    if (field_template[pos_y][pos_x] != T) {
      std::cout << error_msg::POSITION_OCCUPIED << std::endl;
      return false;
    }

    field_template[pos_y][pos_x] = marker;
    std::cout << "successfully set :3" << std::endl;
    stones_set++;
    return true;
  }

  bool player_remove_stone(char marker, Position* pos) {
    int pos_y = get_y_pos(pos->y);
    int pos_x = get_x_pos(pos->x);

    clear_console();

    if (field_template[pos_y][pos_x] != T) {
      std::cout << error_msg::POSITION_NOT_OCCUPIED << std::endl;
      return false;
    }

    field_template[pos_y][pos_x] = marker;
    std::cout << "successfully removed :3" << std::endl;
    stones_set++;
    return true;
  }

  bool check_mill(std::array<std::pair<int, int>, 3> mill, char marker) {

    int counter = 0;

    for (const auto& pos : mill) {
      int pos_y = get_y_pos(pos.second);
      int pos_x = get_x_pos(pos.first);
      if (field_template[pos_y][pos_x] == marker) {
        counter++;
      }
    }
    return counter == 3;
  }

private:
  const int MS = 10;
  static const int X = 26;
  static const int Y = 15;
  static const int STONES = 18;
  static const char T = '#';
  GamePhase current_phase;
  int stones_set;

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
      "#-----------#-----------#",
      "",
      "1   2   3   4   5   6   7"};

  char field_template2[Y][X] = {
      "21---------22----------23",
      "|           |           |",
      "|  18------19------20   |",
      "|   |       |       |   |",
      "|   |  15--16--17   |   |",
      "|   |   |       |   |   |",
      "9--10--11      12--13--14",
      "|   |   |       |   |   |",
      "|   |   6---7---8   |   |",
      "|   |       |       |   |",
      "|   3-------4-------5   |",
      "|           |           |",
      "0-----------1-----------2",
      "",
      "1   2   3   4   5   6   7"};

  int get_y_pos(int pos_y) { return ((7 - (pos_y)) * 2); }

  // irrelevant when saving as [7][7] arr
  int get_x_pos(int pos_x) { return ((pos_x - 1) * 4); }
};
#endif