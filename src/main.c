#include "main.h"

int main(void) {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    do {
        game_t game = init_new_game();
        display_banner();
        printf("\n\n\t\t Please Select Game Mode :"
                "\n\n\t\t    [1]. PLAY WITH COMPUTER"
                "\n\n\t\t    [2]. PLAY WITH A FRIEND"
                "\n\n\t\t    [3]. EXIT PROGRAM  ");
        game.mode   = get_mode();

        switch (game.mode) {
            case SINGLE_PLAYER :
                game.status = single_player(&game);
                break;

            case MULTI_PLAYER :
                game.status = multi_player(&game);
                break;

            case EXIT_PROGRAM :
                return EXIT_SUCCESS;
            
            default : break;
        }

        display_board(game.board);
        if (game.status == WINNER_PL2) {
            //
            if (game.mode == SINGLE_PLAYER) {
                printf("\n\t\t Computer Wins! ");
            }
            else {
                printf("\n\t\t Player 2 Wins! ");
            }
        }
        else if (game.status == WINNER_PL1) {
            printf("\n\t\t Player 1 Wins! ");
        }
        else {
            printf("\n\t\t It's a draw! ");
        }
        cgetch();
    } while (true);
    
    return EXIT_SUCCESS;
}


/**
 * @brief Initialize the 'game_t' struct
*/
game_t init_new_game(void) {
    game_t new_game = {
        .status = ONGOING,
        .player = PLAYER_1,
        .board  = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}
    };
    return new_game;
}


/**
 * @brief Function to check if the board is full
*/
bool is_board_full(char *board) {
    for (size_t cell = 1; cell < BOARD_SIZE; ++cell) {
        if (IS_CELL_EMPTY(board, cell))
            return false;
    }
    return true;
}


/**
 * @brief Check's for winning combinations in the board
 */
bool check_winning_combo(char *board, const int *combination) {
    if (IS_CELL_EMPTY(board, combination[0])) {
        return false;
    }
    
    char symbol = board[combination[0]];
    for (size_t i = 1; i < GRID_LEN; ++i) {
        if (board[combination[i]] != symbol) {
            return false;
        }
    }
    return true;
}


/**
 * @brief Function to check for winner/status of the game
 * @returns
 *      ?  1 IF PLAYER 1 WINS
 *      ?  2 IF PLAYER 2 WINS
 *      ? -1 IF GAME IS IN PROGRESS
 *      ?  O IF GAME ENDED IN A DRAW
 */
int check_winner(char *board) {
    for (size_t i = 0; i < WINNING_COMBOS_COUNT; ++i) {
        if (check_winning_combo(board, WINNING_COMBINATIONS[i])) {
            return (board[WINNING_COMBINATIONS[i][0]] == PLAYER_1_SYMBOL) ? WINNER_PL1 : WINNER_PL2;
        }
    }

    //* Check for a draw
    if (is_board_full(board)) {
        return GAME_DRAW;
    }

    //* No winner yet
    return ONGOING;
}


/**
 * @brief Switches players after every move.
 */
int switch_player (int player, int mode) {
    if (player == PLAYER_1) {
        if (mode == SINGLE_PLAYER) {
            return AUTO_PLAYER;
        }
        return PLAYER_2;
    }
    return PLAYER_1;
}


/**
 * @brief Function to get assigned player symbols.
 */
int get_symbol (int player) {
    return (player == PLAYER_1) ? PLAYER_1_SYMBOL : PLAYER_2_SYMBOL;
}



/**
 * @brief Minimax function to determine the best move for the computer.
 */
int minimax(char *board, int depth, bool is_maximizing) {
    int game_state = check_winner(board);
    if (game_state == WINNER_PL2) {
        return BASE_SCORE - depth; // Computer wins
    }
    if (game_state == WINNER_PL1) {
        return depth - BASE_SCORE; // Player wins
    }
    if (game_state == GAME_DRAW) {
        return GAME_DRAW;
    }

    int best_score = is_maximizing ? INT_MIN : INT_MAX;

    for (size_t cell = 1; cell < BOARD_SIZE; ++cell) {
        if (IS_CELL_EMPTY(board, cell)) {
            char backup = board[cell];
            board[cell] = is_maximizing ? PLAYER_2_SYMBOL : PLAYER_1_SYMBOL;
            int score = minimax(board, depth + 1, !is_maximizing);
            board[cell] = backup;
            best_score = is_maximizing ? (score > best_score ? score : best_score) 
                                       : (score < best_score ? score : best_score);
        }
    }
    return best_score;
}


// Find the best move for the computer
int get_computer_move(char *board) {
    int best_score = INT_MIN;
    int best_move = DEFAULT_MOVE; //? -1

    for (size_t cell = 1; cell < BOARD_SIZE; ++cell) {
        if (IS_CELL_EMPTY(board, cell)) {
            char backup = board[cell];
            board[cell] = PLAYER_2_SYMBOL;
            int score = minimax(board, 0, false);
            board[cell] = backup;
            if (score > best_score) {
                best_score = score;
                best_move = cell;
            }
        }
    }
    return best_move;
}


/**
 * @brief Ehh, multiplayer
 */
int multi_player(game_t *Game) {
    display_board(Game->board);
    do {
        printf("\n\t Player %i enter your move (1-9) : ", Game->player);
        int move = get_move();
        if (!IS_CELL_EMPTY(Game->board, move)) {
            printf("\n\t [!] Invalid move! Please try again!...\n");
            continue;
        }
        else {
            Game->board[move] = get_symbol(Game->player);
            Game->status = check_winner(Game->board);
            Game->player = switch_player(Game->player, MULTI_PLAYER);
        }
        display_board(Game->board);
    } while (Game->status == ONGOING);
    
    return Game->status;
}


/**
 * @brief Play against the computer
 */
int single_player(game_t *Game) {
    display_board(Game->board);
    do {
        int move = DEFAULT_MOVE;
        if (Game->player == AUTO_PLAYER) {
            move = get_computer_move(Game->board);   
        }
        else {
            printf("\n\t Enter your move (1-9) : ");
            move = get_move();
            if (!IS_CELL_EMPTY(Game->board, move)) {
                printf("\n\t [!] Invalid move! Try again.\n");
                continue;
            }
        }
        Game->board[move] = get_symbol(Game->player);
        Game->status = check_winner(Game->board);
        Game->player = switch_player(Game->player, SINGLE_PLAYER);
        display_board(Game->board);
    } while (Game->status == ONGOING);

    return Game->status;
}



/**
 * @brief Print's game banner
 */
void display_banner(void) {
    printf("\033c");
    printf("\n\n\t\t<|====== A SIMPLE TIC TAC TOE GAME ======|>");
    printf("\n\n\t\t    Player 1 (❌)  vs  (🟢) Player 2 ");
}

/**
 * @brief Get's mode of the game
 */
int get_mode(void) {
    do {
        const int c = cgetch();
        if (c == CTRL_K('C')) {
            printf("\n\n\t PROGRAM ABRUPTLY TERMINATED.\n");
            exit(EXIT_FAILURE);
        }
        if (c >= '1' && c <= '3') {
            return c - '0';
        }
    } while (1);
}
/**
 * @brief Get's players move
 */
int get_move(void) {
    do {
        const int c = cgetch();
        if (c == CTRL_K('C')) {
            printf("\n\n\t PROGRAM ABRUPTLY TERMINATED.\n");
            exit(EXIT_FAILURE);
        }
        if (c >= '1' && c <= '9') {
            return c - '0';
        }
    } while (1);
}


/**
 * @brief Prints emojis corresponding to the player symbols
 */
void print_symbol(const char *board, const size_t idx) {
    printf("\t\t\t│");
    for (size_t j = 0; j < GRID_LEN; ++j) {
        const char symbol = board[idx + j];
        if (symbol == PLAYER_1_SYMBOL) {
            printf("  ❌");
        }
        else if (symbol == PLAYER_2_SYMBOL) {
            printf("  🟢");
        }
        else {
            printf("   %c", symbol);
        }
        printf("  │");
    }
    printf("\n");
}
/**
 * @brief FUNCTION TO DRAW BOARD OF THE GAME WITH CELLS MARKED
 */
void display_board(const char *board) {
    display_banner();
    printf("\n\n\t\t\t╭──────┬──────┬──────╮\n");
    for (size_t i = 1; i < BOARD_SIZE; i += GRID_LEN) {
        print_symbol(board, i);
        if (i < BOARD_SIZE - GRID_LEN) {
            printf("\t\t\t├──────┼──────┼──────┤\n");
        }
    }
    printf("\t\t\t╰──────┴──────┴──────╯\n\n");
}


/**
 * @brief cross platform getch function.
 * @author Gavin A. S. Cheng
 * @see https://ascheng.medium.com/linux-getch-for-unix-c2c829721a30
*/
int cgetch(void) {
    #ifdef _WIN32
        return getch();
    #else
        struct termios old_tio, new_tio;

        tcgetattr(STDIN_FILENO, &old_tio);
        new_tio = old_tio;
        new_tio.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

        const int c = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
        return c;
    #endif
}
