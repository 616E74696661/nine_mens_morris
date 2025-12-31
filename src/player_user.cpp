#include "position.cpp"
#include "user.cpp"
#include "mills.cpp"
#include <iostream>
#include <string>

class player_user : public user {
public:
    player_user(std::string name) : user(name) {

    }

    void place_marker(Mills* m) override {
        std::cout << "It's your turn, " << this->name << "!" << std::endl;
        std::cout << "Select marker position:" << std::endl;
        
        int pos_y;
        int pos_x;
        std::cout << "y: ";
        std::cin >> pos_y;
        std::cout << "x: ";
        std::cin >> pos_x;

        try {   
            position pos(pos_x, pos_y);
            m->player_set_stone(marker, &pos);
        }
        catch (const std::string& e) {
            std::cout << e << std::endl;
            place_marker(m); // Retry placing marker
            return;
        }
    }
};