/* Play noughts and crosses (tic-tac-toe) between two human players. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

//-----------------------------------------------------------------------------
// Setting up types and constants.  Don't change this section.

// Constants for player X, player O, and neither N.
enum player { X, O, N };
typedef enum player player;

// Constants to represent validity checking.
enum validity { Unchecked, BadFormat, BadLetter, BadDigit, BadCell, OK };
typedef enum validity validity;

// A game object contains the current state of a game: the number of moves made,
// the player whose turn it is next, and the grid with each cell recording which
// player owns it.
struct game {
    int moves;
    player next;
    player grid[3][3];
};
typedef struct game game;

//-----------------------------------------------------------------------------
// Functions providing the logic of the game.  Develop these.

// Initialize a game, with the given player to move first.
void newGame(game *g, player first) {
    for (int y=0; y<3; y++) {
        for (int x=0; x<3; x++) {
            g->grid[y][x] = N;
        }
    }
    g->next = first;
    g->moves = 0;
}

// Convert the letter in a valid move string such as "b2" into a row index.
int row(char *text) {
    if (text[0] == 'a') return 0;
    if (text[0] == 'b') return 1;
    if (text[0] == 'c') return 2;
    return -1;
}

// Convert the digit in a valid move string such as "b2" into a column index.
int col(char *text) {
    if (text[1] == '1') return 0;
    if (text[1] == '2') return 1;
    if (text[1] == '3') return 2;
    return -1;
}

// Check whether a move string typed in by the user such as "b2" is valid, and
// the corresponding cell is available.  Return a validity code.
validity valid(game *g, char *text) {
    if (strlen(text) != 2) return BadFormat;
    int r = row(text); int c = col(text);
    if (r == -1) return BadLetter;
    if (c == -1) return BadDigit;
    if (g->grid[c][r] == N) return BadCell;
    return OK;
}

// Make a move at the given valid position.
void move(game *g, int r, int c) {
    g->grid[r][c] = g->next;
    g->next = g->next == X ? O : X;
    g->moves++;
}

// Check if the given line is a winning one, and return the winning player.
player line(player p0, player p1, player p2) {
    return p0 == p1 && p0 == p2 ? p0 : N;
}

// Check whether a player has won, and return the winning player.
player win(game *g) {
    for (int r=0; r<3; r++) {
        if (line(g->grid[r][0], g->grid[r][1], g->grid[r][2]) != N) return g->grid[r][0];
    }
    for (int c=0; c<3; c++) {
        if (line(g->grid[0][c], g->grid[1][c], g->grid[2][c]) != N) return g->grid[0][c];
    }
    if (line(g->grid[0][0], g->grid[1][1], g->grid[2][2]) != N) return g->grid[0][0];
    if (line(g->grid[0][2], g->grid[1][1], g->grid[2][0]) != N) return g->grid[0][2];
    return N;
}

// Check whether the game has ended in a draw.
bool draw(game *g) {
    if (win(g) != N) return false;
    for (int r=0; r<3; r++) {
        for (int c=0; c<3; c++) {
            if (g->grid[r][c] == N) return false;
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
// Playing the game: Don't change this section until after submitting.

// Convert a player constant into a character for printing.
char show(player c) {
    return (c == X) ? 'X' : (c == O) ? 'O' : ' ';
}

// Print a validity error message.
void printInvalid(validity v) {
    if (v == BadFormat) printf("Type a letter and a digit");
    else if (v == BadLetter) printf("Type a letter a, b or c");
    else if (v == BadDigit) printf("Type a digit 1, 2 or 3");
    else if (v == BadCell) printf("Choose an empty cell");
    printf("\n");
}

// Display the grid.
void display(game *g) {
}

// Ask the current player for their move, putting it into the given array.
// Ask repeatedly until the user types in a valid move.
void ask(game *g, char text[100]) {
}

// Play the game interactively between two human players who take turns.
void play(char player) {
}

//-----------------------------------------------------------------------------
// Testing and running: Don't change this section.

// Check constants haven't been changed, so that automarking works.
void checkConstants() {
    assert(X == 0 && O == 1 && N == 2);
    assert(Unchecked == 0 && BadFormat == 1 && BadLetter == 2);
    assert(BadDigit == 3 && BadCell == 4 && OK == 5);
}

// Set up a game structure with given contents.  The grid is given as a string
// of the form "XO- -XO X-X", then the next player, then the number of moves.
// Work out the number of moves made and the player to move next.
void setup(game *g, char *grid) {
    int xs = 0, os = 0, moves = 0;
    for (int r=0; r<3; r++) {
        for (int c=0; c<3; c++) {
            char ch = grid[r*4 + c];
            if (ch == 'X') { g->grid[r][c] = X; xs++; moves++; }
            else if (ch == 'O') { g->grid[r][c] = O; os++; moves++; }
            else g->grid[r][c] = N;
        }
    }
    g->moves = moves;
    if (xs == os || xs == os - 1) g->next = X;
    else if (xs == os + 1) g->next = O;
    else printf("Setup problem\n");
}

// Test initialization of the game (tests 1 to 8)
void testNew(game *g) {
    setup(g, "XXO XOO XOX");
    g->next = N;
    newGame(g, X);
    // Test a few of the cells (all should be filled with N).
    assert(g->grid[0][0] == N);
    assert(g->grid[1][1] == N);
    assert(g->grid[2][2] == N);
    assert(g->grid[0][2] == N);
    assert(g->grid[2][1] == N);
    assert(g->next == X);
    assert(g->moves == 0);
    // Test g->next isn't always set to X
    newGame(g, O);
    assert(g->next == O);
}

// Test that the row/col functions give the right answers (tests 9 to 14)
void testRowCol() {
    assert(row("a3") == 0);
    assert(row("b2") == 1);
    assert(row("c2") == 2);
    assert(col("b1") == 0);
    assert(col("b2") == 1);
    // Test pointer comparisons aren't being used
    char text[] = "a3";
    assert(row(text) == 0 && col(text) == 2);

}

// Test invalid input, or trying to play in an occupied cell (tests 15 to 23)
void testInvalid(game *g) {
    newGame(g, X);
    assert(valid(g, "d2") == BadLetter);
    assert(valid(g, "2b") == BadLetter);
    assert(valid(g, "b0") == BadDigit);
    assert(valid(g, "b4") == BadDigit);
    assert(valid(g, "b2x") == BadFormat);
    // Test input "b", but with different values for text[2], in case it is
    // incorrectly being used to test for 'length == 2'.
    assert(valid(g, "b\0x") == BadFormat);
    assert(valid(g, "b\0\0") == BadFormat);
    assert(valid(g, "") == BadFormat);
    setup(g, "---/---/-X-");
    assert(valid(g, "c2") == BadCell);
}

// Test making a move (tests 24 to 29)
void testMove(game *g) {
    newGame(g, X);
    move(g, row("b2"), col("b2"));
    assert(g->grid[1][1] == X);
    assert(g->next == O);
    assert(g->moves == 1);
    move(g, row("a3"), col("a3"));
    assert(g->grid[0][2] == O);
    assert(g->next == X);
    assert(g->moves == 2);
}

// Test the line function (tests 30 to 36)
void testLine() {
    assert(line(X, X, X) == X);
    assert(line(O, O, O) == O);
    assert(line(X, O, O) == N);
    assert(line(O, X, O) == N);
    assert(line(O, O, X) == N);
    assert(line(N, N, N) == N);
    assert(line(X, N, N) == N);
}

// Test winning lines (tests 37 to 44)
void testWin(game *g) {
    setup(g, "XXX -O- -O-");
    assert(win(g) == X);
    setup(g, "-O- XXX -O-");
    assert(win(g) == X);
    setup(g, "-O- -O- XXX");
    assert(win(g) == X);
    setup(g, "O-- OX- O-X");
    assert(win(g) == O);
    setup(g, "-O- XO- -OX");
    assert(win(g) == O);
    setup(g, "--O X-O -XO");
    assert(win(g) == O);
    setup(g, "X-O -XO --X");
    assert(win(g) == X);
    setup(g, "X-O -OX O--");
    assert(win(g) == O);
}

// Test no winning line (tests 45 to 48)
void testNoWin(game *g) {
    setup(g, "--- --- ---");
    assert(win(g) == N);
    setup(g, "O-X XXO OX-");
    assert(win(g) == N);
    setup(g, "XOX XOO OXO");
    assert(win(g) == N);
    setup(g, "OOX XXO OXX");
    assert(win(g) == N);
}

// Test drawn games (tests 49 to 50)
void testDraw(game *g) {
    setup(g, "O-X XXO OX-");
    assert(draw(g) == false);
    setup(g, "OOX XXO OXX");
    assert(draw(g) == true);
}

// Unit testing.
void test() {
    game *g = malloc(sizeof(game));
    checkConstants();
    testNew(g);
    testRowCol();
    testInvalid(g);
    testMove(g);
    testLine();
    testWin(g);
    testNoWin(g);
    testDraw(g);
    free(g);
    printf("All tests passed.\n");
}

// Run the program with no args to carry out the tests, or with one arg (the
// player to go first) to play the game.
int main(int n, char *args[n]) {
    if (n == 1) test();
    else if (n == 2 && strcmp(args[1],"X") == 0) play(X);
    else if (n == 2 && strcmp(args[1],"O") == 0) play(O);
    else {
        fprintf(stderr, "Use: ./oxo  OR  ./oxo X  OR  ./oxo O\n");
        exit(1);
    }
    return 0;
}
