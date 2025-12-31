#include "constants/error_messages.hpp"
#include <vector>

struct position {
    private:
        std::vector<position> valid_positions = {
            position(1,1), position(1,4), position(1,7),
            position(2,2), position(2,4), position(2,6),
            position(3,3), position(3,4), position(3,5),
            position(4,1), position(4,2), position(4,3),
            position(4,5), position(4,6), position(4,7),
            position(5,3), position(5,4), position(5,5),
            position(6,2), position(6,4), position(6,6),
            position(7,1), position(7,4), position(7,7)
        };
    
        bool checkPosition(int xPos, int yPos) {
            for (const auto& pos : valid_positions) {
                if (pos.x == xPos && pos.y == yPos) {
                    return true;
                }
            }
            return false;
        }

    public:
        int x;
        int y;
        position(int xPos, int yPos) {
            if (checkPosition(xPos, yPos)) {
                x = xPos;
                y = yPos;
            } else {
                throw error_msg::INVALID_POSITION;
            }
        }
};

