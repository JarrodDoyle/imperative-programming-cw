
/* Program to play "Rock, Paper, Scissors" in the terminal against a computer */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Valid moves and round result enums
enum { Rock, Paper, Scissors };
enum { Win, Lose, Draw };

// Computer opponent choice
int get_computer_turn(void) {
    return rand() % (Scissors - Rock + 1);
}

// Convert string input to enum value if possible
int convert_choice(const char choice[]) {
    if (strcmp(choice, "Rock\n") == 0) return Rock;
    else if (strcmp(choice, "Paper\n") == 0) return Paper;
    else if (strcmp(choice, "Scissors\n") == 0) return Scissors;
    return -1;
}

// Find out if the player won, lost, or drew
int check_win(int player, int computer) {
    if (player == Rock) {
        if (computer == Scissors) return Win;
        else if (computer == Paper) return Lose;
        else return Draw;
    }
    else if (player == Paper) {
        if (computer == Rock) return Win;
        else if (computer == Scissors) return Lose;
        else return Draw;
    }
    else {
        if (computer == Paper) return Win;
        else if (computer == Rock) return Lose;
        else return Draw;
    }
}

// Player choice
int get_player_turn() {
    int choice = -1;
    char input[32];
    while (choice != Rock & choice != Paper & choice != Scissors) {
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
    while ((player_wins < (rounds + 1) / 2) & (computer_wins < (rounds + 1) / 2)) {
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

int main() {
    char input[16];
    while (strcmp(input, "4\n")) {
        printf("\nWelcome to RPS\n 1. Best of 1\n 2. Best of 3\n 3. Best of 5\n 4. Quit\n\n");
        fgets(input, 16, stdin);
        if (strcmp(input, "1\n") == 0) play_game(1);
        else if (strcmp(input, "2\n") == 0) play_game(3);
        else if (strcmp(input, "3\n") == 0) play_game(5);
    }
    return 0;
}
