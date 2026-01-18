#ifndef GAME_TEXT_HPP
#define GAME_TEXT_HPP

#include <string>
namespace game_text {
static const std::string WELCOME = "Welcome to Muehle!";
static const std::string GAME_MODE = "Please select a game mode:\n1. Player vs Player\n2. Player vs Bot\n9. Load latest gamestate";

static const std::string WIN = "You won! :)";
static const std::string LOSE = "You lost! :(";
static const std::string INVALID_INPUT = "Invalid input";
static const std::string OPTIONS =
    "Press '1' to start playing\nPress '2' to go to the settings";
static const std::string SETTINGS =
    "Press 'a' to set the color of the users\nPress 'b' to";
}; // namespace game_text
#endif