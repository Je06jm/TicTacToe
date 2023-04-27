#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void run_single_player_game() {
    Board_t game_board;
    board_reset(game_board);
    Player_t winner;

    Player_t turn = PLAYER_X;
    Player_t human = PLAYER_NONE;

    char start;
    printf("Do you want to play as X (Y/N) ");
    scanf(" %c", &start);

    if (start == 'Y' || start == 'y') human = PLAYER_X;
    else human = PLAYER_O;

    while ((winner = board_get_winner(game_board)) == PLAYER_NONE) {
        if (turn == human) {
            board_print(game_board);

            printf("It's your turn\n");

            printf("Your move (A/B/C)(1/2/3) ");

            char row, column;
            scanf(" %c%c", &row, &column);

            if (row >= 'A' && row <= 'C') row -= 'A';
            else if (row >= 'a' && row <= 'c') row -= 'a';
            else continue;

            if (column >= '1' && column <= '3') column -= '1';
            else continue;

            if (BOARD_SQUARE(game_board, column, row) != PLAYER_NONE) continue;
            
            BOARD_SQUARE(game_board, column, row) = human;
        } else {
            int x = -1, y = -1;
            board_recommend_move(game_board, turn, &x, &y);

            if (x == -1 || y == -1) {
                printf("Error: Could not find a recommended move\n");
                exit(EXIT_FAILURE);
            }
            
            BOARD_SQUARE(game_board, x, y) = turn;
        }

        turn = turn == PLAYER_X ? PLAYER_O : PLAYER_X;
    }

    board_print(game_board);

    if (winner == PLAYER_TIES) {
        printf("The game is tied\n");
    } else {
        printf("Player ");
        player_print(winner);
        printf(" wins\n");
    }
}

Player_t run_computer_game() {
    Board_t game_board;
    board_reset(game_board);
    Player_t winner;

    while ((winner = board_get_winner(game_board)) == PLAYER_NONE) {
        int x, y;
        board_recommend_move(game_board, PLAYER_X, &x, &y);

        if (x == -1 || y == -1) {
            printf("Error: Could not find a recommended move\n");
            exit(EXIT_FAILURE);
        }

        BOARD_SQUARE(game_board, x, y) = PLAYER_X;

        if ((winner = board_get_winner(game_board)) != PLAYER_NONE) break;

        board_recommend_move(game_board, PLAYER_O, &x, &y);

        if (x == -1 || y == -1) {
            printf("Error: Could not find a recommended move\n");
            exit(EXIT_FAILURE);
        }

        BOARD_SQUARE(game_board, x, y) = PLAYER_O;
    }

    return winner;
}

int main() {
    srand(time(NULL));
    
    printf("Do you want to play? (Y/N) ");
    char play;
    scanf(" %c", &play);

    if (play == 'y' || play == 'Y') run_single_player_game();
    else {
        int games;
        printf("How many games do you want the AI to run? ");
        scanf(" %i", &games);

        if (games <= 0) return 0;
        
        int x_wins = 0, o_wins = 0;
        Player_t winner;
        for (int i = 0; i < games; i++) {
            winner = run_computer_game();
            
            if (winner == PLAYER_X) x_wins++;
            else if (winner == PLAYER_O) o_wins++;
        }

        int ties = games - x_wins - o_wins;
        printf("Played %i games. X won %i times, O won %i times, with %i ties\n", games, x_wins, o_wins, ties);
    }
}