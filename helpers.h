#ifndef HELPERS_H
#define HELPERS_H

#define ROWS 6
#define COLS 7

void printc(int p);
void print_board(int board[ROWS][COLS]);
int is_full(int board[ROWS][COLS]);
int drop_piece(int board[ROWS][COLS], int col, int player);
int check_win(int board[ROWS][COLS], int row, int col, int player);
int score_window(int w[4], int player);
int evaluate(int board[ROWS][COLS], int player);
int minimax(int board[ROWS][COLS], int depth, int is_maximizing);
int best_move(int board[ROWS][COLS], int depth);

#endif