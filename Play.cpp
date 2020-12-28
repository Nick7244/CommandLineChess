/* Derek Chang dchang38 dchang38@jhu.edu
 * Nick Maritato nmarita1 nmarita1@jhu.edu
 * Yunfei Li yli208 yli208@jhu.edu
 * 601.220
 * Final Project
 * 12/08/2017
 */

#include <iostream>
#include <string>
#include "Prompts.h"
#include "Game.h"
#include "ChessGame.h"
#include "KingOfHill.h"
#include "SpookyChessGame.h"

using std::cin;
using std::string;
using std::cout;
using std::stof;
using std::endl;

// Game variant enumeration
enum GameName {STANDARD_CHESS = 1, KING_OF_THE_HILL, SPOOKY_CHESS};


//Determines if game choice input is valid
bool check_game_choice_input(string game_choice_string) {
  if(game_choice_string.length() != 1) {
    return false;
  }else {
    char game_choice_char = game_choice_string[0];
    if(!isdigit(game_choice_char)) {
      return false;
    }else {
      int game_choice = stof(game_choice_string);
      if(game_choice < 1 || game_choice > 3) { 
	return false;
      }else {
	return true;
      }
    }
  }
}
     
	  
// Ask user which game they want to play
int collect_game_choice() {
    Prompts::game_choice();
    string game_choice_string;
    cin >> game_choice_string;
    bool check;
    check = check_game_choice_input(game_choice_string);

    while(check == false) {
      Prompts::game_choice();
      cin >> game_choice_string;
      check = check_game_choice_input(game_choice_string);
    }

    int game_choice = stof(game_choice_string);
    return game_choice;
}

//Determines if new/load input is valid
bool check_new_or_load_choice_input(string new_or_load_string) {
  if(new_or_load_string.length() != 1) {
    return false;
  }else {
    char new_or_load_char = new_or_load_string[0];
    if(!isdigit(new_or_load_char)) {
      return false;
    }else {
      int new_or_load = stof(new_or_load_string);
      if(new_or_load < 1 || new_or_load > 2) { 
	return false;
      }else {
	return true;
      }
    }
  }
}

// Ask user whether to launch new game or load existing one
int determine_new_or_load() {
  Prompts::new_or_load_choice();
  string new_or_load_string;
  cin >> new_or_load_string;
  bool check;
  check = check_new_or_load_choice_input(new_or_load_string);
  
  while(check == false) {
    Prompts::new_or_load_choice();
    cin >> new_or_load_string;
    check = check_new_or_load_choice_input(new_or_load_string);
  }
  
  int new_or_load = stof(new_or_load_string);
  return new_or_load;
}

// Ask user for name of file where game state is stored
string collect_filename() {
    Prompts::load_game();
    string f;
    cin >> f;
    return f;
}

int main() {
  
    // Determine which game to play, and how to begin it
  int game_choice = collect_game_choice();
  int new_or_load_choice = determine_new_or_load();
  bool run_game = true;
  // Set up the desired game
  Game *g = nullptr;
  if (game_choice == STANDARD_CHESS && new_or_load_choice == 1) {  //new standard chess
    g = new ChessGame();
  } else if (game_choice == STANDARD_CHESS && new_or_load_choice == 2) { //load standard chess
    string filename = collect_filename();
    g = new ChessGame(filename, game_choice, run_game);
  } else if (game_choice == KING_OF_THE_HILL && new_or_load_choice == 1) {  //new spooky chess
    g = new KingOfHill();
  } else if (game_choice == KING_OF_THE_HILL && new_or_load_choice == 2) { //load spooky chess
    string filename = collect_filename();
    g = new KingOfHill(filename, game_choice, run_game);
  } else if (game_choice == SPOOKY_CHESS && new_or_load_choice == 1) {  //new spooky chess
    g = new SpookyChessGame();
  } else if (game_choice == SPOOKY_CHESS && new_or_load_choice == 2) { //load spooky chess
    string filename = collect_filename();
    g = new SpookyChessGame(filename, game_choice, run_game);
  } else {
    std::cout << "Invalid option(s) selected. Exiting the program. \n" << std::endl;
    return 1;
  }
  // Begin play of the selected game!
  if(run_game == true)
     g->run(game_choice);
  
  // Nothing else to do here in main, so clean up
  delete g;    
  
  return 0;
}

