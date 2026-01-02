#ifndef POSITION_HPP
#define POSITION_HPP

#include "constants/error_messages.hpp"
#include <array>

struct Position {
public:
  int x;
  int y;

  Position(int xPos, int yPos) {
    if (!isValid(xPos, yPos)) {
      throw error_msg::INVALID_POSITION;
    }
    x = xPos, y = yPos;
  }

  bool isValid(int xPos, int yPos) {
    for (const auto &p : validPositions) {
      if (p.first == xPos && p.second == yPos) {
        return true;
      }
    }
    return false;
  }

private:
  std::array<std::pair<int, int>, 24> validPositions{
      {{1, 1}, {1, 4}, {1, 7}, {2, 2}, {2, 4}, {2, 6}, {3, 3}, {3, 4},
       {3, 5}, {4, 1}, {4, 2}, {4, 3}, {4, 5}, {4, 6}, {4, 7}, {5, 3},
       {5, 4}, {5, 5}, {6, 2}, {6, 4}, {6, 6}, {7, 1}, {7, 4}, {7, 7}}};
};
#endif // POSITION_HPP