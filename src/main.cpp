#include "mills.cpp"
#include "settings.cpp"

int main() {

  Settings setting;
  setting.setup();

  Mills m;
  m.field_output();

  int pos_y;
  int pos_x;
  int player = 0;

  for (int i = 0; i < 10; i++) {
    std::cout << "y: ";
    std::cin >> pos_y;
    std::cout << "x: ";
    std::cin >> pos_x;

    m.player_set_stone(player, pos_y, (int)pos_x);

    m.field_output();
  }
}