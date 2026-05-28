// scores a single window of 4 cells
// player = AI (2), opp = human (1)
int score_window(int w[4], int player) {
    int opp = (player == 1) ? 2 : 1;
    
    int pc = 0;  // player piece count
    int ec = 0;  // empty cell count
    int oc = 0;  // opponent piece count

    // count what's in this window
    for (int i = 0; i < 4; i++) {
        if (w[i] == player)   pc++;
        else if (w[i] == 0)   ec++;
        else                  oc++;
    }

    // score this window based on what's in it
    // remember: positive = good for AI, negative = bad for AI
    if (pc == 4)             return 1000;  // AI wins from this window
    if (pc == 3 && ec == 1)  return 50;   // AI is threatening — one gap to win
    if (pc == 2 && ec == 2)  return 10;   // AI is developing — two gaps
    if (oc == 3 && ec == 1)  return -80;  // human is threatening — must block
    //                         ^^^^ -80 > -10 because blocking is more urgent
    //                              than building your own 2-in-a-row

    return 0; // mixed window — not useful for either player
}

// scores the entire board from AI's perspective
// called by minimax when depth limit is hit and game isn't over
int evaluate(int board[ROWS][COLS], int player) {
    int score = 0;
    int window[4];

    // --- CENTER COLUMN BONUS ---
    // center column (col 3) participates in more possible 4-in-a-rows
    // than any other column (horizontal + both diagonals)
    // so reward AI for having pieces there
    for (int r = 0; r < ROWS; r++)
        if (board[r][COLS/2] == player)
            score += 3;

    // --- HORIZONTAL WINDOWS ---
    // each row has (COLS - 3) = 4 windows of size 4
    // e.g. row 0: [0-3], [1-4], [2-5], [3-6]
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c <= COLS - 4; c++) {
            for (int i = 0; i < 4; i++)
                window[i] = board[r][c + i]; // extract 4 cells
            score += score_window(window, player);
        }

    // --- VERTICAL WINDOWS ---
    // each column has (ROWS - 3) = 3 windows of size 4
    // e.g. col 0: rows[0-3], rows[1-4], rows[2-5]
    for (int c = 0; c < COLS; c++)
        for (int r = 0; r <= ROWS - 4; r++) {
            for (int i = 0; i < 4; i++)
                window[i] = board[r + i][c]; // extract 4 cells going down
            score += score_window(window, player);
        }

    // --- DIAGONAL WINDOWS (down-right) ---
    // goes from top-left to bottom-right  ↘
    // e.g. board[0][0], board[1][1], board[2][2], board[3][3]
    for (int r = 0; r <= ROWS - 4; r++)
        for (int c = 0; c <= COLS - 4; c++) {
            for (int i = 0; i < 4; i++)
                window[i] = board[r + i][c + i];
            score += score_window(window, player);
        }

    // --- DIAGONAL WINDOWS (down-left) ---
    // goes from top-right to bottom-left  ↙
    // e.g. board[0][3], board[1][2], board[2][1], board[3][0]
    // c starts at 3 because we need 3 columns to the left
    for (int r = 0; r <= ROWS - 4; r++)
        for (int c = 3; c < COLS; c++) {
            for (int i = 0; i < 4; i++)
                window[i] = board[r + i][c - i];
            score += score_window(window, player);
        }

    // final score = sum of all window scores + center bonus
    // positive means AI is in a better position
    // negative means human is in a better position
    return score;
}



int minimax(int board[ROWS][COLS], int depth, int is_maximizing) {
    // check if AI already won
    // we check all positions since we don't track last move here
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++) {
            if (board[r][c] == 2 && check_win(board, r, c, 2)) return 1000;
            if (board[r][c] == 1 && check_win(board, r, c, 1)) return -1000;
        }
    if (is_full(board)) return 0;
    if (depth == 0) return evaluate(board, 2); // score from AI's perspective
    if (is_maximizing) { // AI's turn (player 2)
        int best = -99999;
        for (int col = 0; col < COLS; col++) {
            if (board[0][col] != 0) continue; // skip full columns
            int row = drop_piece(board, col, 2);
            int score = minimax(board, depth - 1, 0);
            board[row][col] = 0; // undo
            if (score > best) best = score;
        }
        return best;
    } else { // Human's turn (player 1)
        int best = 99999;
        for (int col = 0; col < COLS; col++) {
            if (board[0][col] != 0) continue;
            int row = drop_piece(board, col, 1);
            int score = minimax(board, depth - 1, 1);
            board[row][col] = 0; // undo
            if (score < best) best = score;
        }
        return best;
    }
} 



        int best_move(int board[ROWS][COLS], int depth) {
    int best_score = -99999;
    int best_col = -1;
    for (int col = 0; col < COLS; col++) {
        if (board[0][col] != 0) continue;
        int row = drop_piece(board, col, 2);
        int score = minimax(board, depth - 1, 0);
        board[row][col] = 0; // undo
        if (score > best_score) {
            best_score = score;
            best_col = col;
        }
    }
    return best_col;
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



// Returns the row the piece lands on, or -1 if column is full
int drop_piece(int board[ROWS][COLS], int col, int player) {
    if (board[0][col] != 0) return -1; // full
    int row = 0;
    while (row < ROWS && board[row][col] == 0)
        row++;
    board[row - 1][col] = player;
    return row - 1;
}



int is_full(int board[ROWS][COLS]) {
    for (int c = 0; c < COLS; c++)
        if (board[0][c] == 0) return 0;
    return 1;
}



void print_board(int board[ROWS][COLS]) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++){
            printc(board[r][c]);
            printf("   ");
        }
        printf("\n\n");
    }
}



void printc(int p){
    if(p==1){
        printf("\033[1;31m\xDB\xDB\033[0m");
    }
    else if(p==2){
        printf("\033[1;33m\xDB\xDB\033[0m");
    }
    else{
        printf("\xDB\xDB");
    }
}
