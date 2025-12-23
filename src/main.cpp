#include "mills.cpp"

int main() {
  Mills m;
  m.ouput_field();

  int pos_y;
  int pos_x;
  int player = 0;

  for (int i = 0; i < 10; i++) {
    std::cout << "x: ";
    std::cin >> pos_x;
    std::cout << "y: ";
    std::cin >> pos_y;

    m.set_stone(player, pos_x, pos_y);

    m.ouput_field();
  }
}