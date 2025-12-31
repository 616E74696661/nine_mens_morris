#ifndef GAME_TEXT_HPP
#define GAME_TEXT_HPP

#include <string>
namespace game_text {
static const std::string WELCOME = "Welcome to Mühle!";
static const std::string GAME_MODE = "Please select a game mode: \r\n1. Player vs Player\r\n2. Player vs Bot";

static const std::string WIN = "you won! :)";
static const std::string LOSE = "you lost! :(";
static const std::string START = "welcome to nine men's morris!";
static const std::string INVALID_INPUT = "invalid input";
static const std::string OPTIONS =
    "press '1' to start playing\npress '2' to go to the settings";
static const std::string SETTINGS =
    "press 'a' to set the color of the players\npress 'b' to";
}; // namespace game_text
#endif