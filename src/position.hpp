#ifndef POSITION_HPP
#define POSITION_HPP

#include "constants/error_messages.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <ostream>
#include <vector>

static std::array<std::pair<int, int>, 24> valid_positions{
    {{1, 1},
     {4, 1},
     {7, 1},
     {2, 2},
     {4, 2},
     {6, 2},
     {3, 3},
     {4, 3},
     {5, 3},
     {1, 4},
     {2, 4},
     {3, 4},
     {5, 4},
     {6, 4},
     {7, 4},
     {3, 5},
     {4, 5},
     {5, 5},
     {2, 6},
     {4, 6},
     {6, 6},
     {1, 7},
     {4, 7},
     {7, 7}}};

static std::vector<std::array<std::pair<int, int>, 3>> valid_mills{
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

static std::array<std::array<int, 4>, 24> adjacency_neighbour = {{
    {1, 9, -1, -1},   // 0: (1,1) → (4,1), (1,4)
    {0, 2, 4, -1},    // 1: (4,1) → (1,1), (7,1), (4,2)
    {1, 14, -1, -1},  // 2: (7,1) → (4,1), (7,4)
    {4, 10, -1, -1},  // 3: (2,2) → (4,2), (2,4)
    {1, 3, 5, 7},     // 4: (4,2) → (4,1), (2,2), (6,2), (4,3)
    {4, 13, -1, -1},  // 5: (6,2) → (4,2), (6,4)
    {7, 11, -1, -1},  // 6: (3,3) → (4,3), (3,4)
    {4, 6, 8, -1},    // 7: (4,3) → (4,2), (3,3), (5,3)
    {7, 12, -1, -1},  // 8: (5,3) → (4,3), (5,4)
    {0, 10, 21, -1},  // 9: (1,4) → (1,1), (2,4), (1,7)
    {3, 9, 11, -1},   // 10: (2,4) → (2,2), (1,4), (3,4)
    {6, 10, 15, -1},  // 11: (3,4) → (3,3), (2,4), (3,5)
    {8, 13, 16, -1},  // 12: (5,4) → (5,3), (6,4), (5,5)
    {5, 12, 14, -1},  // 13: (6,4) → (6,2), (5,4), (7,4)
    {2, 13, 23, -1},  // 14: (7,4) → (7,1), (6,4), (7,7)
    {11, 16, -1, -1}, // 15: (3,5) → (3,4), (4,5)
    {12, 15, 17, 19}, // 16: (4,5) → (5,4), (3,5), (5,5), (4,6)
    {16, 12, -1, -1}, // 17: (5,5) → (4,5), (5,4)
    {10, 19, -1, -1}, // 18: (2,6) → (2,4), (4,6)
    {16, 18, 20, 22}, // 19: (4,6) → (4,5), (2,6), (6,6), (4,7)
    {19, 13, -1, -1}, // 20: (6,6) → (4,6), (6,4)
    {9, 22, -1, -1},  // 21: (1,7) → (1,4), (4,7)
    {19, 21, 23, -1}, // 22: (4,7) → (4,6), (1,7), (7,7)
    {14, 22, -1, -1}  // 23: (7,7) → (7,4), (4,7)
}};

struct Position {
public:
  int x;
  int y;

  Position() {
    x = 0, y = 0;
  }

  Position(int x_pos, int y_pos) {
    if (!is_valid(x_pos, y_pos)) {
      throw error_msg::INVALID_POSITION;
    }
    x = x_pos, y = y_pos;
  }

  bool is_valid(int x_pos, int y_pos) {
    for (const auto& p : valid_positions) {
      if (p.first == x_pos && p.second == y_pos) {
        return true;
      }
    }
    return false;
  }

  static int get_index(int x_pos, int y_pos) {
    int i = 0;
    for (const auto& p : valid_positions) {
      if (p.first == x_pos && p.second == y_pos) {
        return i;
      }
      i++;
    }
    return -1; // error
  }

  static bool is_neighbour(Position pos1, Position pos2) {
    int pos1index = get_index(pos1.x, pos1.y);
    int pos2index = get_index(pos2.x, pos2.y);
    std::array<int, 4> arr = adjacency_neighbour.at(pos1index);
    if (std::find(arr.begin(), arr.end(), pos2index) != arr.end()) {
      printf("%i and %i are neighbours\n", pos1index, pos2index);
      return true;
    }
    printf("%i and %i are NOT neighbours\n", pos1index, pos2index);
    return false;
  }

  std::vector<std::array<std::pair<int, int>, 3>>
  get_possible_mills() {
    std::vector<std::array<std::pair<int, int>, 3>> mills;
    for (const auto& mill : valid_mills) {
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