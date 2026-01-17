#ifndef POSITION_HPP
#define POSITION_HPP

#include "constants/error_messages.hpp"
#include <algorithm>
#include <array>
#include <stdexcept>
#include <tuple>
#include <utility>
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

static std::array<std::array<int, 4>, 24> adjacency_neighbour = {{
    {1, 9, -1, -1},   // 0: (1,1) -> (4,1), (1,4)
    {0, 2, 4, -1},    // 1: (4,1) -> (1,1), (7,1), (4,2)
    {1, 14, -1, -1},  // 2: (7,1) -> (4,1), (7,4)
    {4, 10, -1, -1},  // 3: (2,2) -> (4,2), (2,4)
    {1, 3, 5, 7},     // 4: (4,2) -> (4,1), (2,2), (6,2), (4,3)
    {4, 13, -1, -1},  // 5: (6,2) -> (4,2), (6,4)
    {7, 11, -1, -1},  // 6: (3,3) -> (4,3), (3,4)
    {4, 6, 8, -1},    // 7: (4,3) -> (4,2), (3,3), (5,3)
    {7, 12, -1, -1},  // 8: (5,3) -> (4,3), (5,4)
    {0, 10, 21, -1},  // 9: (1,4) -> (1,1), (2,4), (1,7)
    {3, 9, 11, 18},   // 10: (2,4) -> (2,2), (1,4), (3,4), (2,6)
    {6, 10, 15, -1},  // 11: (3,4) -> (3,3), (2,4), (3,5)
    {8, 13, 17, -1},  // 12: (5,4) -> (5,3), (6,4), (5,5)
    {5, 12, 14, 20},  // 13: (6,4) -> (6,2), (5,4), (7,4), (6,6)
    {2, 13, 23, -1},  // 14: (7,4) -> (7,1), (6,4), (7,7)
    {11, 16, -1, -1}, // 15: (3,5) -> (3,4), (4,5)
    {15, 17, 19, -1}, // 16: (4,5) -> (3,5), (5,5), (4,6)
    {12, 16, -1, -1}, // 17: (5,5) -> (4,5), (5,4)
    {10, 19, -1, -1}, // 18: (2,6) -> (2,4), (4,6)
    {16, 18, 20, 22}, // 19: (4,6) -> (4,5), (2,6), (6,6), (4,7)
    {13, 19, -1, -1}, // 20: (6,6) -> (4,6), (6,4)
    {9, 22, -1, -1},  // 21: (1,7) -> (1,4), (4,7)
    {19, 21, 23, -1}, // 22: (4,7) -> (4,6), (1,7), (7,7)
    {14, 22, -1, -1}  // 23: (7,7) -> (7,4), (4,7)
}};

struct Position {
private:
  bool is_valid(int x_pos, int y_pos) {
    for (const auto& p : valid_positions) {
      if (p.first == x_pos && p.second == y_pos) {
        return true;
      }
    }
    return false;
  }

public:
  int x;
  int y;

  Position() {
    x = 0, y = 0;
  }

  Position(int x_pos, int y_pos) {
    if (!is_valid(x_pos, y_pos)) {
      throw std::invalid_argument(error_msg::INVALID_POSITION);
    }
    x = x_pos, y = y_pos;
  }

  virtual ~Position() = default;

  bool operator==(const Position& other) const {
    return x == other.x && y == other.y;
  }

  bool operator<(const Position& other) const {
    return std::tie(x, y) < std::tie(other.x, other.y);
  }

  bool is_valid() {
    return is_valid(x, y);
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

  static bool is_adjacent_position(Position pos1, Position pos2) {
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

  std::vector<Position*> get_adjacent_positions() {

    std::vector<Position*> neighbours;

    // Get adjacent indexes of position
    int pos_index = get_index(x, y);
    std::array<int, 4> neighbour_indexes = adjacency_neighbour.at(pos_index);

    // Push back valid positions
    std::pair<int, int> pos_coord;
    for (auto index : neighbour_indexes) {
      if (index != -1) {
        pos_coord = valid_positions.at(index);
        neighbours.push_back(new Position(pos_coord.first, pos_coord.second));
      }
    }
    return neighbours;
  }
};

static std::vector<Position> get_valid_positions() {
  std::vector<Position> positions;
  for (std::pair<int, int> pos_pair : valid_positions) {
    positions.push_back(Position(pos_pair.first, pos_pair.second));
  }
  return positions;
}

static bool contains(std::vector<Position> v, Position p) {
  return std::find(v.begin(), v.end(), p) != v.end();
}

#endif // POSITION_HPP