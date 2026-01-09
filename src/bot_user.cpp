#include "user.cpp"

class bot_user : public user {
public:
  bot_user(std::string name) : user(name) {
  }

  Position place_marker() override {
    std::cout << "Bot " << this->name << " is placing a marker." << std::endl;
    // Simple bot logic: place marker at the first available position
    return Position(0, 0);
  }
};