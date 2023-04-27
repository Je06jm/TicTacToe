#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

typedef enum {
    PLAYER_NONE,
    PLAYER_X,
    PLAYER_O,
    PLAYER_TIES
} Player_t;

typedef Player_t Board_t[9];

#define BOARD_SQUARE(board, x, y) board[y * 3 + x]

#define BOARD_LOSES -1
#define BOARD_TIES 0
#define BOARD_WINS 1

void player_print(Player_t player);
void board_reset(Board_t board);
void board_print(Board_t board);
void board_copy(Board_t dst, Board_t src);
void board_recommend_move(Board_t board, Player_t turn, int* x, int* y);
int board_score(Board_t board, Player_t player, bool minimize);
Player_t board_get_winner(Board_t board);

#endif