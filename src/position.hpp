#ifndef POSITION_HPP
#define POSITION_HPP

#include "constants/error_messages.hpp"
#include <array>
#include <vector>

static std::array<std::pair<int, int>, 24> validPositions{
    {{1, 1}, {1, 4}, {1, 7}, {2, 2}, {2, 4}, {2, 6}, {3, 3}, {3, 4}, {3, 5}, {4, 1}, {4, 2}, {4, 3}, {4, 5}, {4, 6}, {4, 7}, {5, 3}, {5, 4}, {5, 5}, {6, 2}, {6, 4}, {6, 6}, {7, 1}, {7, 4}, {7, 7}}};

static std::vector<std::array<std::pair<int, int>, 3>> validMills{
    {{{1, 1}, {1, 4}, {1, 7}}},
    {{{2, 2}, {2, 4}, {2, 6}}},
    {{{3, 3}, {3, 4}, {3, 5}}},
    {{{4, 1}, {4, 2}, {4, 3}}},
    {{{4, 5}, {4, 6}, {4, 7}}},
    {{{5, 3}, {5, 4}, {5, 5}}},
    {{{6, 2}, {6, 4}, {6, 6}}},
    {{{7, 1}, {7, 4}, {7, 7}}},
    {{{1, 1}, {4, 1}, {7, 1}}},
    {{{2, 2}, {4, 2}, {6, 2}}},
    {{{3, 3}, {4, 3}, {5, 3}}},
    {{{1, 4}, {2, 4}, {3, 4}}},
    {{{5, 4}, {6, 4}, {7, 4}}},
    {{{3, 5}, {4, 5}, {5, 5}}},
    {{{2, 6}, {4, 6}, {6, 6}}},
    {{{1, 7}, {4, 7}, {7, 7}}}};

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
    for (const auto& p : validPositions) {
      if (p.first == xPos && p.second == yPos) {
        return true;
      }
    }
    return false;
  }

  std::vector<std::array<std::pair<int, int>, 3>> getPossibleMills() {
    std::vector<std::array<std::pair<int, int>, 3>> mills;
    for (const auto& mill : validMills) {
      for (const auto& pos : mill) {
        if (pos.first == x && pos.second == y) {
          mills.push_back(mill);
        }
      }
    }
    return mills;
  }
};

#endif // POSITION_HPP