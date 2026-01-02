#include "user.cpp"

class bot_user : public user {
public:
    bot_user(std::string name) : user(name) {

    }

    void place_marker(Mills* m) override {
        std::cout << "Bot " << this->name << " is placing a marker." << std::endl;
        // Simple bot logic: place marker at the first available position      
    }
};