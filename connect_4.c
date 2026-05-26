#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 6
#define COLS 7

void print_board(int board[ROWS][COLS]) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++)
            printf("%d ", board[r][c]);
        printf("\n");
    }
}

int is_full(int board[ROWS][COLS]) {
    for (int c = 0; c < COLS; c++)
        if (board[0][c] == 0) return 0;
    return 1;
}

// Returns the row the piece lands on, or -1 if column is full
int drop_piece(int board[ROWS][COLS], int col, int player) {
    if (board[0][col] != 0) return -1; // full
    int row = 0;
    while (row < ROWS && board[row][col] == 0)
        row++;
    board[row - 1][col] = player;
    return row - 1;
}

int check_win(int board[ROWS][COLS], int row, int col, int player) {
    int r, c, count;

    // Horizontal
    count = 1;
    for (c = col-1; c >= 0 && board[row][c] == player; c--) count++;
    for (c = col+1; c < COLS && board[row][c] == player; c++) count++;
    if (count >= 4) return 1;

    // Vertical
    count = 1;
    for (r = row+1; r < ROWS && board[r][col] == player; r++) count++;
    // no need to check up — pieces always stack downward
    if (count >= 4) return 1;

    // Diagonal down-right / up-left
    count = 1;
    for (r=row-1, c=col-1; r>=0 && c>=0 && board[r][c]==player; r--,c--) count++;
    for (r=row+1, c=col+1; r<ROWS && c<COLS && board[r][c]==player; r++,c++) count++;
    if (count >= 4) return 1;

    // Diagonal down-left / up-right
    count = 1;
    for (r=row-1, c=col+1; r>=0 && c<COLS && board[r][c]==player; r--,c++) count++;
    for (r=row+1, c=col-1; r<ROWS && c>=0 && board[r][c]==player; r++,c--) count++;
    if (count >= 4) return 1;

    return 0;
}

int main() {
    srand(time(NULL));

    int board[ROWS][COLS] = {0};
    int current_player = rand() % 2 + 1; // 1 or 2
    int col_inp, row;

    printf("Starting player: %d\n\n", current_player);
    print_board(board);

    while (1) {
        printf("Player %d enter column (1-7): ", current_player);
        scanf("%d", &col_inp);
        col_inp--; // convert to 0-indexed

        if (col_inp < 0 || col_inp >= COLS) {
            printf("Invalid column.\n");
            continue;
        }

        row = drop_piece(board, col_inp, current_player);
        if (row == -1) {
            printf("Column is full, try again.\n");
            continue;
        }

        print_board(board);

        if (check_win(board, row, col_inp, current_player)) {
            printf("Player %d wins!\n", current_player);
            return 0;
        }

        if (is_full(board)) {
            printf("It's a draw!\n");
            return 0;
        }

        current_player = (current_player == 1) ? 2 : 1;
    }
}