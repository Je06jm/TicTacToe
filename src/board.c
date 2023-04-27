#include "board.h"

#include <stdio.h>
#include <stdlib.h>

void player_print(Player_t player) {
    printf("%c", player == PLAYER_X ? 'X' : player == PLAYER_O ? 'O' : '.');
}

void board_print(Board_t board) {
    printf("  1 2 3\n");

    const char letters[3] = { 'A', 'B', 'C' };
    
    for (int y = 0; y < 3; y++) {
        printf("%c ", letters[y]);
        
        for (int x = 0; x < 3; x++) {
            player_print(BOARD_SQUARE(board, x, y));
            printf(" ");
        }

        printf("\n");
    }
}

void board_reset(Board_t board) {
    for (int i = 0; i < 9; i++) {
        board[i] = PLAYER_NONE;
    }
}

void board_copy(Board_t dst, Board_t src) {
    for (int i = 0; i < 9; i++) {
        dst[i] = src[i];
    }
}

void board_recommend_move(Board_t board, Player_t turn, int* x, int* y) {
    int best_score = BOARD_LOSES - 1;
    Board_t next_board;
    board_copy(next_board, board);

    int score;
    
    int valid_x[9];
    int valid_y[9];
    int valid_positions = 0;

    bool is_valid;

    for (int y_pos = 0; y_pos < 3; y_pos++) {
        for (int x_pos = 0; x_pos < 3; x_pos++) {
            if (BOARD_SQUARE(next_board, x_pos, y_pos) != PLAYER_NONE) continue;

            is_valid = false;

            BOARD_SQUARE(next_board, x_pos, y_pos) = turn;

            score = board_score(next_board, turn, true);

            if (score > best_score) {
                best_score = score;
                
                valid_positions = 0;
                is_valid = true;
            } else if (score == best_score) {
                is_valid = true;
            }

            BOARD_SQUARE(next_board, x_pos, y_pos) = PLAYER_NONE;

            if (is_valid) {
                valid_x[valid_positions] = x_pos;
                valid_y[valid_positions] = y_pos;
                valid_positions++;
            }
        }
    }

    if (valid_positions == 0) {
        while (true) {
            *x = rand() % 3;
            *y = rand() % 3;

            if (BOARD_SQUARE(next_board, *x, *y) == PLAYER_NONE) break;
        }
    } else {
        int selected_pos = rand() % valid_positions;

        *x = valid_x[selected_pos];
        *y = valid_y[selected_pos];
    }
}

int board_score(Board_t board, Player_t player, bool minimize) {
    Player_t winner = board_get_winner(board);
    if (winner != PLAYER_NONE) {
        if (winner == PLAYER_TIES) return 0;
        if (winner == player) return BOARD_WINS;
        return BOARD_LOSES;
    }
    
    Board_t next_board;
    board_copy(next_board, board);

    Player_t selected_player;
    if (minimize) selected_player = player == PLAYER_X ? PLAYER_O : PLAYER_X;
    else selected_player = player;

    int score;
    if (minimize) score = BOARD_WINS;
    else score = BOARD_LOSES;

    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (BOARD_SQUARE(next_board, x, y) != PLAYER_NONE) continue;

            BOARD_SQUARE(next_board, x, y) = selected_player;

            int current = board_score(next_board, player, !minimize);

            if (minimize && current < score) score = current;
            else if (!minimize && current > score) score = current;

            BOARD_SQUARE(next_board, x, y) = PLAYER_NONE;
        }
    }

    return score;
}

Player_t board_get_winner(Board_t board) {
    for (int i = 0; i < 3; i++) {
        if (BOARD_SQUARE(board, i, 0) != PLAYER_NONE &&
            BOARD_SQUARE(board, i, 0) == BOARD_SQUARE(board, i, 1) &&
            BOARD_SQUARE(board, i, 0) == BOARD_SQUARE(board, i, 2)) return BOARD_SQUARE(board, i, 0);
        
        if (BOARD_SQUARE(board, 0, i) != PLAYER_NONE &&
            BOARD_SQUARE(board, 0, i) == BOARD_SQUARE(board, 1, i) &&
            BOARD_SQUARE(board, 0, i) == BOARD_SQUARE(board, 2, i)) return BOARD_SQUARE(board, 0, i);
    }

    if (BOARD_SQUARE(board, 0, 0) != PLAYER_NONE &&
        BOARD_SQUARE(board, 0, 0) == BOARD_SQUARE(board, 1, 1) &&
        BOARD_SQUARE(board, 0, 0) == BOARD_SQUARE(board, 2, 2)) return BOARD_SQUARE(board, 0, 0);
    
    if (BOARD_SQUARE(board, 2, 0) != PLAYER_NONE &&
        BOARD_SQUARE(board, 2, 0) == BOARD_SQUARE(board, 1, 1) &&
        BOARD_SQUARE(board, 2, 0) == BOARD_SQUARE(board, 0, 2)) return BOARD_SQUARE(board, 2, 0);
    
    for (int i = 0; i < 9; i++) {
        if (board[i] == PLAYER_NONE) return PLAYER_NONE;
    }

    return PLAYER_TIES;
}