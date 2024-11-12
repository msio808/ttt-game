#ifndef SRC_H
#define SRC_H

#include <ctype.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
#endif

enum PlayerOptions {
    PLAYER_1        = 0x01,
    PLAYER_2        = 0x02,
    AUTO_PLAYER     = 0x03,
    PLAYER_1_SYMBOL = 0x58, //? 'X'
    PLAYER_2_SYMBOL = 0x4F  //? 'O'
};

enum GameOptions {
    //* Game status
    ONGOING         = -0x01,
    GAME_DRAW       =  0x00,
    WINNER_PL1      =  0x01,
    WINNER_PL2      =  0x02,
    //* Minimax
    BASE_SCORE      =  0x0A,
    DEFAULT_MOVE    = -0x01,
    //* Game board size
    GRID_LEN        =  0x03,
    BOARD_SIZE      =  0x0A,
    //* Game Modes
    EXIT_PROGRAM    =  0x03,
    MULTI_PLAYER    =  0x02,
    SINGLE_PLAYER   =  0x01,
    
    //* WINNING COMBINATIONS COUNT
    WINNING_COMBOS_COUNT = 0x08
};

static const int WINNING_COMBINATIONS[WINNING_COMBOS_COUNT][GRID_LEN] = {
    {1, 2, 3}, {4, 5, 6}, {7, 8, 9},
    {1, 4, 7}, {2, 5, 8}, {3, 6, 9},
    {1, 5, 9}, {3, 5, 7}
};

typedef struct GAME {
    int  mode;
    int  status;
    int  player;
    char board[BOARD_SIZE];
} game_t;

int cgetch(void);
int get_mode(void);
int get_move(void);
int get_symbol (int);
int switch_player (int, int);

game_t init_new_game(void);
int multi_player(game_t *);
int single_player(game_t *);

void display_banner(void);
void display_board(const char *);
void print_symbol(const char *, const size_t);

int  check_winner(char *);
bool is_board_full(char *);
bool check_winning_combo(char *, const int *);

int get_computer_move(char *);
int minimax(char *, int, bool);

#define RESET "\033[0m"
#define GREEN "\033[38;2;0;255;0m"
#define RED   "\033[38;2;255;0;0m"

#define CTRL_K(key) (key & 0x1F)
#define IS_CELL_EMPTY(board, cell) ((board)[cell] == (char)(cell) + '0')


#endif // !SRC_H
