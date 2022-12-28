#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

//question 5: using structure instead of global variable
struct Gamestate {
    int marbles_in_middle = -1;
    int human_marbles     = -1;
    int computer_marbles  = -1;

    int total_games = 0; 
    int human_wins = 0; 
    int computer_wins = 0; 
}; 
Gamestate game;

//question 2
enum class Player {
    human,
    computer
};
Player whose_turn;

//question 1: welcome screen
void welcome_screen() {
  cout<<"+-----------------------+"<<endl;
  cout << "| Welcome to Even Wins! |\n";
  cout<<"+-----------------------+"<<endl;
  cout << "Even Wins is a two-person game. You start with\n";
  cout << "27 marbles in the middle of the table.\n";
  cout << "\n";
  cout << "Players alternate taking marbles from the middle.\n";
  cout << "A player can take 1 to 4 marbles on their turn, and\n";
  cout << "turns cannot be skipped. The game ends when there are\n";
  cout << "no marbles left, and the winner is the one with an even\n";
  cout << "number of marbles.";
  cout << "\n";
}

string marbles_str(int n) {
  if (n == 1) return "1 marble";
  return to_string(n) + " marbles";
}

void choose_first_player() {
 for (;;) {
    cout << "\nDo you want to play first? (y/n) --> ";
    string ans;
    cin >> ans;
    if (ans == "y") {
        whose_turn = Player::human;
        return;
    } else if (ans == "n") {
        whose_turn = Player::computer;
        return;
    } else {
      cout << "\nPlease enter 'y' if you want to play first,\n";
        cout << "or 'n' if you want to play second.\n";
    }
  }
} // choose_first_player

//question 6: using operator for if else
void next_player() {
  whose_turn = (whose_turn == Player::human) ? Player::computer : Player::human;
}

void print_board() {
  cout << "\n";
  cout << " marbles in the middle: " << game.marbles_in_middle << " " << string(game.marbles_in_middle, '*') << "\n";
  cout << "    # marbles you have: " << game.human_marbles << "\n";
  cout << "# marbles computer has: " << game.computer_marbles << "\n";
  cout <<  "\n";
}

// returns true if s is an int, and false otherwise
// https://www.cplusplus.com/reference/string/stoi/
bool is_int(const string& s) {
  try {
    stoi(s); // throws an invalid_argument exception if conversion fails
    return true;
  } catch (...) {
    return false;
  }
}

void human_turn() {
  // get number in range 1 to min(4, marbles_in_middle)
  int max_choice = min(4, game.marbles_in_middle);
  cout << "It's your turn!\n";
  for(;;) {
    cout << "Marbles to take? (1 - " << max_choice << ") --> ";
    string s;
    cin >> s;
    if (!is_int(s)) {
      cout << "\n  Please enter a whole number from 1 to " << max_choice
      << "\n";
      continue;
    }

    // convert s to an int so it can be compared
    int n = stoi(s);
    if (n < 1) {
      cout << "\n  You must take at least 1 marble\n";
      continue;
    }
    if (n > max_choice) {
      cout << "\n  You can take at most " << marbles_str(max_choice)
      << "\n";
      continue;
    }

    cout << "\nOkay, taking " << marbles_str(n) << " ...\n";
    game.marbles_in_middle -= n;
    game.human_marbles += n;

    return;
  } // for
} // human_turn

//question 7: better AI 
/* explanation: the ai takes 2 marbles until it is down to 5 or 4, where then AI will take 4 to take the obvious win. aside from 5 or 4, as the ai only adds 2, an even number, there is no way it loses. it is a very simple strategy but not fun for the player*/
void computer_turn() {
  cout << "It's the computer's turn ...\n";
  int max_choice = min(4, game.marbles_in_middle);

  int n=2;

  if (game.marbles_in_middle==5 || game.marbles_in_middle==4){
    n=4;
  }

  cout << "Computer takes " << marbles_str(n) << " ...\n";
  game.marbles_in_middle -= n;
  game.computer_marbles += n;
}

//question 4: randomizing taunts 
string random_taunts(){
  int random = rand()%5;
  string taunts[5]{"tremble before it's mighty brain!", 
  "hahahaha i am the winner",
  "you can try again but just know that you are going to lose",
  "muahahaha get destroyed",
  "nice try but you will never beat me" }; 
  
  return taunts[random];
}

void game_over() {
  cout << "\n";
  cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
   << "!! All the marbles are taken: Game Over !!\n"
   << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";
  print_board();
  if (game.human_marbles % 2 == 0) {
    cout << "You are the winner! Congratulations!\n";
    game.human_wins++;
    game.total_games++;
  } else {
    cout << "The computer wins: "<<random_taunts()<<endl;
    game.computer_wins++;
    game.total_games++;
  }
}

void play_game() {
  // initialize the game state
  game.marbles_in_middle = 27;
  game.human_marbles = 0;
  game.computer_marbles = 0;
  print_board();

  for (;;) {
    if (game.marbles_in_middle == 0) {
      game_over();
      return;
    } else if (whose_turn == Player::human) {
      human_turn();
      print_board();
      next_player();
    } else if (whose_turn == Player::computer) {
      computer_turn();
      print_board();
      next_player();
    }
  } // for
} // play_game

//question 3
void stats() {
  cout<<"\nStatistics"<<endl;
  cout<<"----------"<<endl;
  cout<<"total games: "<<game.total_games<<endl;
  cout<<"human wins: "<<game.human_wins<<endl;
  cout<<"computer wins: "<<game.computer_wins;
}

int main() {
  srand(time(NULL));

  welcome_screen();

  for(;;) {
    choose_first_player();
    play_game();
    stats();

    // ask if the user if they want to play again
    cout << "\nWould you like to play again? (y/n) --> ";
    string again;
    cin >> again;
    if (again == "y") {
      cout << "\nOk, let's play again ...\n";
    } else {
      cout << "\nOk, thanks for playing ... goodbye!\n";
      return 0;
    }
  } // for
} // main
