#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>

//? Player Numbers
#define PLAYER_1 1
#define PLAYER_2 2

//? Game status
#define ONGOING -1
#define GAME_DRAW 0
#define WINNER_PL1 PLAYER_1
#define WINNER_PL2 PLAYER_2


#define GRID_LEN 3
//* Game board size
#define BOARD_SIZE 10
//* Define player symbols
#define PLAYER_1_SYMBOL 'X'
#define PLAYER_2_SYMBOL 'O'

//* WINNING COMBINATIONS COUNT
#define WINNING_COMBOS_COUNT 8 

//* Define winning combinations as a 2D array
const int WINNING_COMBINATIONS[WINNING_COMBOS_COUNT][GRID_LEN] = {
    {1, 2, 3}, {4, 5, 6}, {7, 8, 9},   //? Horizontal
    {1, 4, 7}, {2, 5, 8}, {3, 6, 9},   //? Vertical
    {1, 5, 9}, {3, 5, 7}               //? Diagonal
};


int  cgetch(void);
void printBoard(char *);
int  checkWinner(char *);
bool allEqual(char *, size_t);
bool checkWinningCombo(char *, const int *);

int main(void) {
    int choice;
    int player = PLAYER_1;
    int gameStatus = ONGOING;
    char square[BOARD_SIZE] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    do {
        //* Print the board
        printBoard(square);

        //* Get the player's move
        printf("\tPlayer %i, enter a number : ", player);
        scanf("%i", &choice);

        //* Update the board with the player's move
        char symbol = (player == PLAYER_1) ? PLAYER_1_SYMBOL : PLAYER_2_SYMBOL;
        if (choice >= 1 && choice <= 9 && *(square + choice) == choice + '0') {
            *(square + choice) = symbol;
            gameStatus = checkWinner(square);
            //* Switch to the other player
            player = (player == PLAYER_1) ? PLAYER_2 : PLAYER_1;
        } else {
            printf("\tInvalid move! Please try again!...");
            cgetch();
        }
    } while (gameStatus == ONGOING);

    //* Print the final board
    printBoard(square);

    //* Print the game result
    if (gameStatus == GAME_DRAW) {
        printf("It's a draw!\n");
        cgetch();
    } else {
        printf("Player %i wins!\n", gameStatus);
        cgetch();
    }

    return 0;
}


// TODO : Function to check if all elements in an array are equal
bool allEqual(char *arr, size_t size) {
    for (size_t i = 0; i < size - 1; ++i) {
        if (*(arr + i) != *(arr + (i + 1)))
            return false;
    }

    return true;
}

// TODO : Function to check if all elements in a winning combination are the same and not empty
bool checkWinningCombo(char *board, const int *combination) {
    char symbol = *(board + *(combination + 0));

    if (symbol == ' ')
        return false;

    for (size_t i = 1; i < GRID_LEN; ++i) {
        if (*(board + *(combination + i)) != symbol)
            return false;
    }
    return true;
}
/*
 TODO : FUNCTION TO RETURN GAME STATUS
 ? 1 PLAYER 1 WINS
 ? 2 PLAYER 2 WINS
 ? -1 FOR GAME IS IN PROGRESS
 ? O GAME ENDED IN A DRAW
*/
int checkWinner(char *square) {
    //* Check each winning combination
    for (size_t i = 0; i < WINNING_COMBOS_COUNT; ++i) {
        if (checkWinningCombo(square, *(WINNING_COMBINATIONS + i)))
            return (*(square + (*(*(WINNING_COMBINATIONS + i) + 0))) == PLAYER_1_SYMBOL) ? WINNER_PL1 : WINNER_PL2;
    }

    //* Check for a draw
    if (allEqual(square + 1, BOARD_SIZE - 1))
        return GAME_DRAW;

    //* No winner yet
    return ONGOING;
}

// TODO : FUNCTION TO DRAW BOARD OF THE GAME WITH PLAYERS MARKED
void printBoard(char *square) {
    //* Clear the console screen for a fresh display & print banner.
	printf("\033c");

    //* Print banner and player symbols.
    printf("\n\n\t\t<|====== A SIMPLE TIC TAC TOE GAME ======|>\n\n");
    printf("\t\t\tPlayer 1 (%c)  -  Player 2 (%c)\n\n\n",
        PLAYER_1_SYMBOL, PLAYER_2_SYMBOL);

    //* Print the game board using loops
    for (size_t i = 1; i < BOARD_SIZE; i += GRID_LEN) {
        printf("\t\t\t     |     |     \n");
        printf("\t\t\t  %c  |  %c  |  %c \n",
            *(square + i), *(square + (i + 1)), *(square + (i + 2)));
        
        if (i < BOARD_SIZE - GRID_LEN)
            printf("\t\t\t_____|_____|_____\n");
        else
            printf("\t\t\t     |     |     \n");
    }
    printf("\n");
}

/* 
    TODO : getch function for unix system.
?   Gets a character from the user without echoing to the terminal.
*/
int cgetch(void) {
    struct termios old_tio, new_tio;
    int c;

    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    c = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    return c;
}
