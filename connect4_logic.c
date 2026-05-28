#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include"helpers.h"
#include"ai_helper_func.c"

#define ROWS 6
#define COLS 7


int main() {
    srand(time(NULL));

    int board[ROWS][COLS] = {0};
    int current_player = rand() % 2 + 1; // 1 or 2
    int col_inp, row;

    printf("Starting player: %d\n\n", current_player);
    print_board(board);

   while (1) {
    if (current_player == 1) {
        // Human turn
        printf("Your turn — enter column (1-7): ");
        scanf("%d", &col_inp);
        col_inp--;
        if (col_inp < 0 || col_inp >= COLS) { printf("Invalid.\n"); continue; }
        row = drop_piece(board, col_inp, 1);
        if (row == -1) { printf("Column full, try again.\n"); continue; }
    } else {
        // AI turn
        printf("AI is thinking...\n");
        col_inp = best_move(board, 4); // depth 4 — change this later
        row = drop_piece(board, col_inp, 2);
        printf("AI chose column %d\n", col_inp + 1);
    }

    print_board(board);

    if (check_win(board, row, col_inp, current_player)) {
        printf("%s wins!\n", current_player == 1 ? "You" : "AI");
        return 0;
    }
    if (is_full(board)) { printf("Draw!\n"); return 0; }

    current_player = (current_player == 1) ? 2 : 1;
}
}