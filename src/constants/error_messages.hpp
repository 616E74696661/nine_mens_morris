#ifndef ERROR_MESSAGES_HPP
#define ERROR_MESSAGES_HPP

#include <string>

namespace error_msg {
static const std::string CREATE_GAME_ERROR = "Error while creating game";
static const std::string TOO_MANY_PLAYERS_ERROR = "Error while creating users";
static const std::string POSITION_OCCUPIED = "Invalid move, position occupied. Please try again";
static const std::string POSITION_NOT_OCCUPIED = "Invalid move, position not occupied. Please try again";
static const std::string NOT_YOUR_STONE = "Invalid move, not allowed to move the stone of opponent. Please try again";
static const std::string INVALID_POSITION = "Invalid position coordinates";
static const std::string NOT_NEIGHBOUR = "New position is not a neighbour. Please try again.";
static const std::string INVALID_SELECTION = "Invalid selection, please try again";
static const std::string INVALID_INPUT_TYPE = "Input has to be a number. Please try again.";
static const std::string INVALID_MOVE = "Invalid move, please try again";
static const std::string TOO_MANY_PLAYERS = "Too many users, maximum is 2";
static const std::string INVALID_REMOVAL = "Invalid removal, please select an opponent's marker.";
static const std::string BOT_PLACE_FAIL = "Bot failed to place stone.";
static const std::string INVALID_NUMBER_OF_STONES = "There is an invalid number of stones from this player on the board.";
static const std::string STONE_PART_OF_MILL = "Cannot remove this stone because it is part of a completed mill. You may only remove stones from a completed mill, if there is no other option.";
}; // namespace error_msg
#endif