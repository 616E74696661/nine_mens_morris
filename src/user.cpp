#include "constants/error_messages.hpp"
#include "constants/game_text.hpp"
#include "iostream";

static int number = 0;

struct position {
    private:
        int x;
        int y;
    public:
        position(int xPos, int yPos) {
            x = xPos;
            y = yPos;
        }
        
};


typedef struct user_t {
    private:
        char marker;
        void set_marker() {
            if (number == 0) {
                marker = 'O';
            } else if (marker == 1){
                marker = 'X';
            } else {
                throw error_msg::TOO_MANY_PLAYERS_ERROR;
            }
            number++;
        }
    public:
        user_t() {
            set_marker();
        }

        void make_move() {
            std::cout << "It's your turn Player 1:" << std::endl;
            std::cout << "Select marker:" << std::endl;
            
        }

        void place_marker() {

        }

        
} user;