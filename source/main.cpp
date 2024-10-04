#include <grrlib.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>
#include "contb.h"
#include "player1_point.h"
#include <algorithm>
#include <cstdio>

#define EMPTY ' '
#define COMPUTER 'X'
#define PLAYER 'O'
#define SIZE 3

typedef struct {
    char array[SIZE][SIZE];
} Board;

typedef struct {
    int row;
    int col;
} Cursor;

GRRLIB_ttfFont *fontTexture;
GRRLIB_texImg *player1Pointer;

// Colors
const u32 GRRLIB_WHITE = 0xFFFFFFFF;
const u32 GRRLIB_BLACK = 0x000000FF;

void initialize_board(Board *board) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board->array[i][j] = EMPTY;
        }
    }
}

void draw_board(Board *board, Cursor *cursor, int ir_x, int ir_y) {
    //char ir_coordinates[50];
    //sprintf(ir_coordinates, "IR Coordinates: x = %d, y = %d", ir_x, ir_y);

    GRRLIB_FillScreen(GRRLIB_WHITE);

    int cell_size = 100;
    int offset_x = (640 - SIZE * cell_size) / 2;
    int offset_y = (480 - SIZE * cell_size) / 2;

    for (int i = 0; i <= SIZE; i++) {
        GRRLIB_Line(offset_x + i * cell_size, offset_y, offset_x + i * cell_size, offset_y + SIZE * cell_size, GRRLIB_BLACK);
        GRRLIB_Line(offset_x, offset_y + i * cell_size, offset_x + SIZE * cell_size, offset_y + i * cell_size, GRRLIB_BLACK);
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int x = offset_x + j * cell_size + cell_size / 2;
            int y = offset_y + i * cell_size + cell_size / 2;
            if (board->array[i][j] == COMPUTER) {
                GRRLIB_PrintfTTF(x - 10, y - 20, fontTexture, "X", 32, GRRLIB_BLACK);
            } else if (board->array[i][j] == PLAYER) {
                GRRLIB_PrintfTTF(x - 10, y - 20, fontTexture, "O", 32, GRRLIB_BLACK);
            }
        }
    }

    // Draw IR coordinates on the screen
    //GRRLIB_PrintfTTF(50, 400, fontTexture, ir_coordinates, 24, GRRLIB_BLACK);

    // Adjust the IR pointer position to center the texture
    int adjusted_ir_x = ir_x - (player1Pointer->w / 2);
    int adjusted_ir_y = ir_y - (player1Pointer->h / 2);

    // Draw cursor at the adjusted IR position
    if (ir_x >= 0 && ir_x <= 640 && ir_y >= 0 && ir_y <= 480) {
        GRRLIB_DrawImg(adjusted_ir_x, adjusted_ir_y, player1Pointer, 0, 1, 1, GRRLIB_WHITE);
    }

    GRRLIB_Render();
}

int full(Board *board) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board->array[i][j] == EMPTY) {
                return 0;
            }
        }
    }
    return 1;
}

int check_win(Board *board, char symbol) {
    for (int i = 0; i < SIZE; i++) {
        if ((board->array[i][0] == symbol && board->array[i][1] == symbol && board->array[i][2] == symbol) ||
            (board->array[0][i] == symbol && board->array[1][i] == symbol && board->array[2][i] == symbol)) {
            return 1;
        }
    }
    if ((board->array[0][0] == symbol && board->array[1][1] == symbol && board->array[2][2] == symbol) ||
        (board->array[0][2] == symbol && board->array[1][1] == symbol && board->array[2][0] == symbol)) {
        return 1;
    }
    return 0;
}

int evaluate(Board *board) {
    if (check_win(board, COMPUTER)) {
        return 10;
    } else if (check_win(board, PLAYER)) {
        return -10;
    } else {
        return 0;
    }
}

int minimax(Board *board, int depth, int isMaximizing) {
    int score = evaluate(board);

    if (score == 10 || score == -10) {
        return score;
    }

    if (full(board)) {
        return 0;
    }

    if (isMaximizing) {
        int best = -1000;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board->array[i][j] == EMPTY) {
                    board->array[i][j] = COMPUTER;
                    best = std::max(best, minimax(board, depth + 1, 0));
                    board->array[i][j] = EMPTY;
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board->array[i][j] == EMPTY) {
                    board->array[i][j] = PLAYER;
                    best = std::min(best, minimax(board, depth + 1, 1));
                    board->array[i][j] = EMPTY;
                }
            }
        }
        return best;
    }
}

void best_move(Board *board, Cursor *cursor) {
    int bestVal = -1000;
    cursor->row = -1;
    cursor->col = -1;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board->array[i][j] == EMPTY) {
                board->array[i][j] = COMPUTER;
                int moveVal = minimax(board, 0, 0);
                board->array[i][j] = EMPTY;
                if (moveVal > bestVal) {
                    cursor->row = i;
                    cursor->col = j;
                    bestVal = moveVal;
                }
            }
        }
    }
}

void title_screen() {
    while (true) {
        WPAD_ScanPads();  // Scan the Wiimotes
        u32 pressed = WPAD_ButtonsDown(0);  // Get the buttons pressed on the first Wiimote

        // Clear the screen with a white background
        GRRLIB_FillScreen(GRRLIB_WHITE);

        // Draw a black rectangle as the background for the title text
        //GRRLIB_Rectangle(40, 40, 360, 100, GRRLIB_BLACK, true);

        // Print the title text
        GRRLIB_PrintfTTF(60, 60, fontTexture, "Wii Tic-Tac-Toe!", 32, GRRLIB_BLACK);

        // Print the instruction text
        GRRLIB_PrintfTTF(60, 110, fontTexture, "Press A to start", 24, GRRLIB_BLACK);

        GRRLIB_Render();  // Render the frame buffer to the TV

        // Wait for the user to press the A button to start the game
        if (pressed & WPAD_BUTTON_A) {
            break;
        }
    }
}

void game_over_screen(int winner, int draw) {
    while (1) {
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);

        if (pressed & WPAD_BUTTON_A) {
            break;
        } else if (pressed & WPAD_BUTTON_B) {
            title_screen();
            break;
        }

        GRRLIB_FillScreen(GRRLIB_WHITE);

        if (winner == PLAYER) {
            GRRLIB_PrintfTTF(50, 50, fontTexture, "Player wins!", 32, GRRLIB_BLACK);
        } else if (winner == COMPUTER) {
            GRRLIB_PrintfTTF(50, 50, fontTexture, "Computer wins!", 32, GRRLIB_BLACK);
        } else if (draw) {
            GRRLIB_PrintfTTF(50, 50, fontTexture, "It's a draw!", 32, GRRLIB_BLACK);
        }

        GRRLIB_PrintfTTF(50, 100, fontTexture, "Press A to restart, B to go back to the title screen", 24, GRRLIB_BLACK);

        GRRLIB_Render();
    }
}

int main(int argc, char **argv) {
    // Initialize GRRLIB
    GRRLIB_Init();
    
    // Initialize the Wii remote
    WPAD_Init();
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);  // Enable IR data
    WPAD_SetVRes(WPAD_CHAN_0, 640, 480);  // Set virtual resolution for the IR

    // Load font and pointer texture
    fontTexture = GRRLIB_LoadTTF((const u8*)contb_ttf, contb_ttf_length);
    player1Pointer = GRRLIB_LoadTexture((const u8*)player1_point_png);

    title_screen();

    // Create and initialize the game board
    Board board;
    initialize_board(&board);

    // Initialize cursor
    Cursor cursor = {0, 0};

    int playerTurn = 1;
    int winner = 0;
    int draw = 0;

    // Main game loop
    while (true) {
        // Scan the Wii remote
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        
        // Read the IR data
        ir_t ir;
        WPAD_IR(WPAD_CHAN_0, &ir);
        
        // Adjust the IR coordinates for fine-tuning
        int ir_x = ir.sx * 1.05 - player1Pointer->w / 0.45;
        int ir_y = ir.sy * 1.05 - player1Pointer->h / 0.45;

        // Draw the game board and IR pointer
        draw_board(&board, &cursor, ir_x, ir_y);
        
        // Check for exit
        if (pressed & WPAD_BUTTON_HOME) {
            break;  // Exit the loop and end the program
        }

        if (playerTurn) {
            if (pressed & WPAD_BUTTON_A) {
                int cell_size = 100;
                int offset_x = (640 - SIZE * cell_size) / 2;
                int offset_y = (480 - SIZE * cell_size) / 2;
                int col = (ir_x - offset_x) / cell_size;
                int row = (ir_y - offset_y) / cell_size;
                if (row >= 0 && row < SIZE && col >= 0 && col < SIZE && board.array[row][col] == EMPTY) {
                    board.array[row][col] = PLAYER;
                    playerTurn = 0;
                    if (check_win(&board, PLAYER)) {
                        winner = PLAYER;
                    } else if (full(&board)) {
                        draw = 1;
                    }
                }
            }
        }

        if (!playerTurn && !winner && !draw) {
            best_move(&board, &cursor);
            board.array[cursor.row][cursor.col] = COMPUTER;
            playerTurn = 1;
            if (check_win(&board, COMPUTER)) {
                winner = COMPUTER;
            } else if (full(&board)) {
                draw = 1;
            }
        }

        if (winner || draw) {
            game_over_screen(winner, draw);
            initialize_board(&board);
            cursor.row = 0;
            cursor.col = 0;
            playerTurn = 1;
            winner = 0;
            draw = 0;
        }
    }

    // Free resources and exit
    GRRLIB_FreeTTF(fontTexture);
    GRRLIB_FreeTexture(player1Pointer);
    GRRLIB_Exit();
    return 0;
}

