#ifndef GAME_TEXT_HPP
#define GAME_TEXT_HPP

#include <string>
namespace game_text {
static const std::string WELCOME =
    "\nWelcome to Nine Men's Morris!\n\n"
    "GOAL:\tGet 3 stones in a row to remove an opponent's stone.\n"
    "\tWin by reducing opponent to 2 stones or blocking all moves.\n\n"
    "PHASES:\n"
    "\tOpening: Place your 9 stones on the board\n"
    "\tMidgame: Move stones to adjacent positions\n"
    "\tEndgame: With 3 stones left, jump anywhere\n\n"
    "Type 'stop' to quit. Game auto-saves after each turn.\n";
static const std::string GAME_MODE_NO_FILE = "Please select a game mode:\n1. Player vs Player\n2. Player vs Bot\n";
static const std::string GAME_MODE = GAME_MODE_NO_FILE + "3. Load last saved game\n";
static const std::string NO_FILE_FOUND = "No saved game found";
static const std::string PVP_SELECTED = "Player vs Player mode selected.";
static const std::string PVBOT_SELECTED = "Player vs Player mode selected.";
}; // namespace game_text
#endif