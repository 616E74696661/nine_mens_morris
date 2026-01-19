# nine_mens_morris

## game rules / phases

- 2 users, 9 pieces per user
- pieces are placed and moved on the line crossing of the board
- whenever three pieces of the same user are places in a straight row, a mill is closed and one opponent piece has to be removed
- the goal of the game is to reduce the opponent to only two pieces or to surround the opponent pieces in such a way that there are no valid moves for the opponent

### opening

- each user places one piece on an unoccupied position on the board in turns
- if a mill is closed by setting a piece, the user may take one of the opponent's pieces

### midgame

- once all pieces are set, the midgame starts
- each user moves one piece along the lines to a free, neighboring position
- if a mill is closed by moving a piece, the user may take one of the opponent's pieces
- a user must move a piece in EACH turn
- if there is no legal move, the user has lost

### endgame

- if a user has only three pieces left, they can jump with one piece to any unoccupied position instead of moving only along the board lines

source: https://nine-mens-morris.net/manual.html

## starting the game
Run ```g++ src/main.cpp -o src/main.exe``` to compile the game

Run ```src/mainn.exe``` to start the game


## files overview

### main.cpp
Game loop controller managing turn flow and win conditions. Coordinates between opening phase (placement), midgame/endgame (movement), handles mill detection after each move and triggers stone removal when mills are formed.

### user.hpp
Abstract base class defining the player interface. Tracks stone counts (placed, removed, on board) and enforces the contract for placing stones, moving stones and removing opponent pieces that all player types must implement.

### bot_user.cpp
Bot implementation with strategic decision-making. Prioritizes completing own mills, blocking opponent mills and falls back to random valid moves when no tactical advantage exists. Uses the mill detection system to make informed choices.

### player_user.cpp
Human player implementation handling console input/output. Validates user inputs for stone placement and movement, enforces game rules (adjacency for normal moves, mill protection when removing stones) and provides error handling with rollback capability.

### field.hpp
Manages the game board representation and validation. Handles stone placement/removal, position validation and board state display. Contains the visual template of the mills board and coordinate translation logic.

### mill.hpp
Defines all valid mill configurations and provides mill detection logic. Checks for completed mills, identifies potential mills that can be formed or blocked and determines which opponent stones can be removed.

### position.hpp
Represents board positions with coordinate validation and adjacency relationships. Maintains the adjacency graph defining which positions connect to each other, essential for validating stone movement in the midgame phase.

### settings.cpp
Handles game initialization and mode selection. Creates player objects based on chosen mode (PvP/PvBot), manages save file loading for continuing previous games and initializes the game state.

### data_io.hpp
Saves and loads game state to/from a binary file. Serializes the complete game state including board configuration, game mode, player stone counts and turn information to enable game resumption.

### helper.hpp
Utility functions for input validation and console management. Provides safe integer input reading with error handling and cross-platform console clearing functionality.