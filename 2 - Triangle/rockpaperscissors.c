/* Program to play "Rock, Paper, Scissors" in the terminal against a computer */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Valid moves and round result enums
enum { Rock, Paper, Scissors };
enum { Win, Lose, Draw };

// Computer opponent choice
int get_computer_turn(void) {
    return rand() % (Scissors - Rock + 1);
}

// Convert string input to enum value if possible. Case insensitive input.
int convert_choice(const char choice[]) {
    char lower[16];
    // Convert all char in string to lower case
    for (int i=0; i<=strlen(choice); i++) {
        if (i == strlen(choice)) lower[i] = choice[i];
        else lower[i] = tolower(choice[i]);
    }
    if (strcmp(lower, "rock\n") == 0) return Rock;
    else if (strcmp(lower, "paper\n") == 0) return Paper;
    else if (strcmp(lower, "scissors\n") == 0) return Scissors;
    return -1;
}

// Find out if the player won, lost, or drew
int check_win(int player, int computer) {
    if (player == Rock) {
        if (computer == Scissors) return Win;
        else if (computer == Paper) return Lose;
        else if (computer == Rock) return Draw;
    }
    else if (player == Paper) {
        if (computer == Rock) return Win;
        else if (computer == Scissors) return Lose;
        else if (computer == Paper) return Draw;
    }
    else if (player == Scissors) {
        if (computer == Paper) return Win;
        else if (computer == Rock) return Lose;
        else if (computer == Scissors) return Draw;
    }
    // If no return by this point, one or more inputs are invalid
    return -1;
}

// Player choice
int get_player_turn() {
    int choice = -1;
    char input[32];
    while (choice != Rock && choice != Paper && choice != Scissors) {
        printf("Please choose Rock, Paper, or Scissors:\n");
        fgets(input, 32, stdin);
        choice = convert_choice(input);
    }
    return choice;
}

// Play a single round of RPS. Keeps going until someone wins.
int play_round() {
    int player;
    int comp;
    int result;
    player = get_player_turn();
    comp = get_computer_turn();
    result = check_win(player, comp);
    printf("Rock...Paper...Scissors!\n");
    // Print player choice
    printf("You played %s\n", player == Rock ? "Rock" : player == Paper ? "Paper" : "Scissors");
    // Print computer choice
    printf("Computer played %s\n", comp == Rock ? "Rock": comp == Paper ? "Paper" : "Scissors");
    // Print whether the player won, lost, or drew
    printf("You %s\n", result == Win ? "won" : result == Lose ? "lost" : "drew");
    return result;
}

// Play a full game of RPS. Allows for Best of Series
int play_game(int rounds) {
    int player_wins = 0;
    int computer_wins = 0;
    int result;
    // While no one has won more than half of max rounds (no one has won series)
    while ((player_wins < (rounds + 1) / 2) && (computer_wins < (rounds + 1) / 2)) {
        printf("\nThe score is player %d, computer %d\n", player_wins, computer_wins);
        result = play_round();
        if (result == Win) player_wins++;
        else if (result == Lose) computer_wins++;
    }
    if (player_wins > computer_wins) {
        printf("You beat the computer %d:%d!\n", player_wins, computer_wins);
    }
    else if (computer_wins > player_wins) {
        printf("You lost to the computer %d:%d!\n", player_wins, computer_wins);
    }
    else return -1;
    return 0;
}

// A replacement for the library assert function.
void assert(int line, bool b) {
    if (b) return;
    printf("The test on line %d fails.\n", line);
    exit(1);
}

// Test possible "Rock" inputs
void test_rock() {
    assert(__LINE__, convert_choice("Rock\n") == Rock);
    assert(__LINE__, convert_choice("rock\n") == Rock);
    assert(__LINE__, convert_choice("rOCk\n") == Rock);
    assert(__LINE__, convert_choice("Rock") == -1);
    assert(__LINE__, convert_choice("rock") == -1);
    assert(__LINE__, convert_choice("potato") == -1);
}

// Test possible "Paper" inputs
void test_paper() {
    assert(__LINE__, convert_choice("Paper\n") == Paper);
    assert(__LINE__, convert_choice("paper\n") == Paper);
    assert(__LINE__, convert_choice("pAPeR\n") == Paper);
    assert(__LINE__, convert_choice("Paper") == -1);
    assert(__LINE__, convert_choice("paper") == -1);
    assert(__LINE__, convert_choice("Orange") == -1);
}

// Test possible "Scissors" inputs
void test_scissors() {
    assert(__LINE__, convert_choice("Scissors\n") == Scissors);
    assert(__LINE__, convert_choice("scissors\n") == Scissors);
    assert(__LINE__, convert_choice("SCissOrs\n") == Scissors);
    assert(__LINE__, convert_choice("Scissors") == -1);
    assert(__LINE__, convert_choice("scissors") == -1);
    assert(__LINE__, convert_choice("False") == -1);
}

// Test check_win function
void test_check_win() {
    assert(__LINE__, check_win(Rock, Paper) == Lose);
    assert(__LINE__, check_win(Rock, Rock) == Draw);
    assert(__LINE__, check_win(Rock, Scissors) == Win);
    assert(__LINE__, check_win(Rock, 12) == -1);
    assert(__LINE__, check_win(Paper, Rock) == Win);
    assert(__LINE__, check_win(Paper, Paper) == Draw);
    assert(__LINE__, check_win(Paper, Scissors) == Lose);
    assert(__LINE__, check_win(Paper, -1) == -1);
    assert(__LINE__, check_win(Scissors, Rock) == Lose);
    assert(__LINE__, check_win(Scissors, Paper) == Win);
    assert(__LINE__, check_win(Scissors, Scissors) == Draw);
    assert(__LINE__, check_win(Scissors, 85) == -1);
    assert(__LINE__, check_win(19, 21) == -1);
    assert(__LINE__, check_win(19, Rock) == -1);
    assert(__LINE__, check_win(19, Paper) == -1);
    assert(__LINE__, check_win(10, Scissors) == -1);
}

// Run all tests
void test() {
    test_rock();
    test_paper();
    test_scissors();
    test_check_win();
    printf("All tests passed\n");
}

int main(int n, char** args) {
    // If "test" argument in terminal run tests
    if (n == 2 && strcmp(args[1], "test") == 0) test();
    // Otherwise just run game normally
    else {
        char input[16];
        // Main menu UI + input
        while (strcmp(input, "4\n")) {
            printf("\nWelcome to RPS\n 1. Best of 1\n 2. Best of 3\n 3. Best of 5\n 4. Quit\n\n");
            fgets(input, 16, stdin);
            if (strcmp(input, "1\n") == 0) play_game(1);
            else if (strcmp(input, "2\n") == 0) play_game(3);
            else if (strcmp(input, "3\n") == 0) play_game(5);
        }
    }
    return 0;
}
