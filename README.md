# Wii Tic-Tac-Toe

This is a Tic-Tac-Toe game for the Nintendo Wii, developed using **GRRLIB** for graphics rendering and the **Wii Remote's IR sensor** for pointer-based controls. The game features a single-player mode where the player competes against a computer AI powered by the **Minimax algorithm**.

## Features
- Play Tic-Tac-Toe against a challenging AI opponent.
- Use the Wii Remote's infrared sensor to point and select positions on the board.
- The AI utilizes the Minimax algorithm to make optimal moves.
- Simple and intuitive gameplay interface with pointer-controlled input.
- Title screen and game-over screen for smooth transitions.

## Demo Video
[![Wii Tic-Tac-Toe Demo](https://img.youtube.com/vi/Wix70jcytD8/maxresdefault.jpg)](https://www.youtube.com/watch?v=Wix70jcytD8)

Watch the full demo on [YouTube](https://www.youtube.com/watch?v=Wix70jcytD8).

## Controls
- **Wii Remote (IR Pointer)**: Use the IR pointer to select cells on the board.
- **A Button**: Place your move in an empty cell.
- **Home Button**: Exit the game.
- **A Button (on Game Over screen)**: Restart the game.
- **B Button (on Game Over screen)**: Return to the title screen.

## How to Play
1. The game starts at the **title screen**. Press **A** on your Wii Remote to begin.
2. Use the IR pointer to hover over the Tic-Tac-Toe grid, and press **A** to place your mark (O).
3. The computer AI will take its turn (X) after you.
4. The game ends when either player wins or the board is full (draw).
5. After the game, you can restart or return to the title screen from the **Game Over screen**.

## How it Works

### Minimax Algorithm
The AI uses the **Minimax algorithm** to calculate the best possible move. This algorithm evaluates all possible game states recursively, assuming both the player and the AI play optimally. It either maximizes the AI's chances of winning or minimizes the player's chances of winning, depending on whose turn it is.

### Wii Remote Integration
The IR pointer functionality allows players to select positions on the Tic-Tac-Toe board using the Wii Remote, giving a smooth and intuitive experience. The game adjusts the pointer's position to center the cursor texture at the IR coordinates.

### Board Representation
The game board is represented as a 3x3 grid. The player's moves are represented by 'O' and the computer's by 'X'. The game checks for win conditions after each move, and if the board is full, it declares a draw.

## Installation and Usage

### Requirements
- A **Nintendo Wii** with **Homebrew Channel** installed OR
- **Dolphin Emulator**.
- **DevKitPro** toolchain for Wii development.
- **GRRLIB** library for graphics.

### Compiling
1. Clone the repository:
    ```bash
    git clone https://github.com/Phi1lS/WiiTicTacToe.git
    cd wii-tic-tac-toe
    ```
2. Make sure you have **DevKitPro** and **GRRLIB** installed.
3. Compile the game using `make`:
    ```bash
    make
    ```
4. Transfer the resulting `.dol` file to your Wii and run it via the **Homebrew Channel**.

### Dependencies
- [DevKitPro](https://devkitpro.org/)
- [GRRLIB](https://grrlib.santo.fr/)

## Files

- `main.cpp`: Main game loop and logic.
- `contb.h` and `player1_point.h`: Resources for the game's font and pointer textures.
- `Makefile`: Build script for the project.
- `GRRLIB`: Library used for rendering graphics.

## Credits
- **GRRLIB**: Open-source library used for Wii graphics rendering.
- **DevKitPro**: Toolchain for Wii development.
- **Minimax Algorithm**: Used for AI decision-making in the game.