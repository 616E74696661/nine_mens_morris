#ifndef ERROR_MESSAGES_HPP
#define ERROR_MESSAGES_HPP

#include <string>
namespace error_msg {
static const std::string CREATE_GAME_ERROR = "error while creating game";
static const std::string TOO_MANY_PLAYERS_ERROR = "error on creating players";
static const std::string INVALID_SELECTION = "invalid selection, please try again";
static const std::string INVALID_POSITION = "invalid position coordinates";
const std::string INVALID_MOVE = "invalid move, please try again";
const std::string TOO_MANY_PLAYERS = "too many players, maximum is 2";
};
#endif