/* Derek Chang dchang38 dchang38@jhu.edu
 * Nick Maritato nmarita1 nmarita1@jhu.edu
 * Yunfei Li yli208 yli208@jhu.edu
 * 601.220
 * Final Project
 * 12/08/2017
 */

#include <iostream>
#include <cassert>
#include <cctype>
#include <sstream>
#include <string>

#include "Game.h"
#include "Prompts.h"
#include "Piece.h"
#include "ChessGame.h"
#include "KingOfHill.h"
#include "SpookyChessGame.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::stof;
using std::istringstream;

enum GameName {STANDARD_CHESS = 1, KING_OF_THE_HILL, SPOOKY_CHESS};

Game::~Game() {

    // Delete the factories used to generate pieces
    for (size_t i = 0; i < _registered_factories.size(); i++) {
        delete _registered_factories[i];
    }

    // Delete any other dynamically-allocated resources here
    for(unsigned int i = 0; i < _pieces.size(); i++) {
      delete _pieces[i];
    }
}

// Create a Piece on the board using the appropriate factory.
// Returns true if the piece was successfully placed on the board.
bool Game::init_piece(int piece_type, Player owner, Position pos) {
    Piece* piece = new_piece(piece_type, owner);
    if (!piece) return false;

    // Fail if the position is out of bounds
    if (!valid_position(pos)) {
        Prompts::out_of_bounds();
        return false;
    }

    // Fail if the position is occupied
    if (get_piece(pos)) {
        Prompts::blocked();
        return false;
    }
    _pieces[index(pos)] = piece;
    return true;
}

// Get the Piece at a specified Position.  Returns nullptr if no
// Piece at that Position or if Position is out of bounds.
Piece* Game::get_piece(Position pos) const {
    if (valid_position(pos))
        return _pieces[index(pos)];
    else {
        Prompts::out_of_bounds();
        return nullptr;
    }
}
// Function to get the symbol fot the input piece
string Game::get_piece_type(int piece_type, Player p) {
  if(p == 0 || p == 1) {
      if(piece_type == 0) {
	return "P";
      } else if(piece_type == 1) {
	return "R";
      } else if(piece_type == 2) {
	return "N";
      } else if(piece_type == 3) {
	return "B";
      } else if(piece_type == 4) {
	return "Q";
      } else if(piece_type == 5) {
	return "K";
      }
  } else if(p == 2) {
    if(piece_type == 6) {
      return "G";
    }
  }
  return "x";
}

// Function to display the board
void Game::display_board() {
  Terminal::Color color = Terminal::BLUE;
  Player owner = WHITE;
  string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  for(int i = (int)_height + 1; i > 0; i--) { // loop through each row
    for(int j = (int)_width + 1; j > 0; j--) { // loop through each column
      if(i == (int)_height + 1) { // first row is all letters for columns
	if(j == 9) { // first letter column
	  int x = _width - (j - 1);
	  cout << "    " << alpha[x];
	} else if(j == 1) { // if reached end of grid
	  cout << "   " << endl;
	  cout << "  _______________________________________________  " << endl;
	} else {
	  int x = _width - (j - 1);
	  cout << "     " << alpha[x]; // print letter for column
	}
      } else { // not in first row
	if(j == (int)_width + 1) { // first value should be row number
	  cout << " |";
	  if(color == Terminal::BLUE) //check for first time through
	    color = Terminal::GREEN;
	  Terminal::color_bg(color);
	  cout << "     ";
	  for(int k = 1; k < (int)_width; k++){
	    Terminal::set_default();
	    cout << "|";
	    if(color == Terminal::B_RED) 
	      color = Terminal::GREEN;
	    else
	      color = Terminal::B_RED;
	    Terminal::color_bg(color);
	    cout << "     ";
	    Terminal::set_default();
	  }
	  cout << "|" << endl;
	  cout << i << "|";
	} else {
	  Position p(_width - j, i - 1); // get position
	  Piece *piece = get_piece(p); // get piece in position
	  string result = " ";
	  if(piece != NULL) { // empty space if no piece at position
	    // get information about piece
	    int type = piece->piece_type();
	    owner = piece->owner();
	    result = get_piece_type(type, owner); // get symbol to display
	  }
	  if(j != 1) { // all spaces not at edge
	    color = Terminal::GREEN;
	    if((i+j)%2 != 0) 
	      color = Terminal::B_RED;
	    Terminal::color_bg(color);
	    cout << "  ";
	    Terminal::Color text = Terminal::BLACK;
	    if(owner == WHITE && result != " ") { // piece is white
	      text = Terminal::WHITE;
	      Terminal::color_fg(true, text);
	    }if(owner == BLACK && result != " ") { // piece is black
	      text = Terminal::BLACK;
	      Terminal::color_fg(false, text);
	    }
	    if(owner == NO_ONE && result != " ") { // piece is ghost
	      text = Terminal::BLUE;
	      Terminal::color_fg(false, text);
	    }
	    char esc = 27;
	    cout << esc << "[2m" << result << esc << "[1m" << "  ";
	    Terminal::set_default();
	    cout << "|";
	  } else { // space at edge
	    color = Terminal::GREEN;
	    if((i+j)%2 != 0) 
	      color = Terminal::B_RED;
	    Terminal::color_bg(color);
	    cout << "  ";
	    Terminal::Color text = Terminal::BLACK;
	    if(owner == WHITE && result != " ") {
	      text = Terminal::WHITE;
	      Terminal::color_fg(true, text);
	    }if(owner == BLACK && result != " ") {
	      text = Terminal::BLACK;
	      Terminal::color_fg(false, text);
	    }
	    if(owner == NO_ONE && result != " ") {
	      text = Terminal::BLUE;
	      Terminal::color_fg(false, text);
	    }
	    cout << result;
	    cout << "  ";
	    Terminal::set_default();
	    cout << "|" << endl;
	    cout << " |";
	    if(color == Terminal::B_RED) 
	      color = Terminal::GREEN;
	    else
	      color = Terminal::B_RED;
	    Terminal::color_bg(color);
	    cout << "_____";
	    for(int x = 1; x < (int)_width; x++) {
	      Terminal::set_default();
	      cout << "|";
	      if(color == Terminal::B_RED) 
		color = Terminal::GREEN;
	      else
		color = Terminal::B_RED;
	      Terminal::color_bg(color);
	      cout << "_____";
	    }
	    Terminal::set_default();
	    cout << "|" << endl;
	  }
	}
      }
    }
  }
}

/* Function to print the appropriate prompt for the result. */
void Game::get_result(int result) {
  if(result == PARSE_ERROR) {
    Prompts::parse_error();
    cout << endl;
  } else if(result == MOVE_ERROR_OUT_OF_BOUNDS) {
    Prompts::out_of_bounds();
    cout << endl;
  } else if(result == MOVE_ERROR_NO_PIECE) {
    Prompts::no_piece();
    cout << endl;
  } else if(result == MOVE_ERROR_ILLEGAL) {
    Prompts::illegal_move();
    cout << endl;
  } else if(result == MOVE_ERROR_BLOCKED) {
    Prompts::blocked();
    cout << endl;
  } else if(result == MOVE_ERROR_CANT_CASTLE) {
    Prompts::cannot_castle();
    cout << endl;
  } else if(result == MOVE_ERROR_MUST_HANDLE_CHECK) {
    Prompts::must_handle_check();
    cout << endl;
  } else if(result == MOVE_ERROR_CANT_EXPOSE_CHECK) {
    Prompts::cannot_expose_check();
    cout << endl;
  } else if(result == SUCCESS) { // piece moves successfully
    cout << endl;
    _turn++; // increment turn
    _num_rand_gen++; // increment number of random number generations
  } else if(result == MOVE_CAPTURE) { // piece attacked another successfully
    Prompts::capture(player_turn());
    cout << endl;
    _turn++;
    _num_rand_gen++;    
  } else if(result == MOVE_CHECK) {
    if(player_turn() == WHITE) 
      Prompts::check(BLACK);
    if(player_turn() == BLACK) 
      Prompts::check(WHITE);
    cout << endl;
    _turn++;
    _num_rand_gen++;
  } else if(result == MOVE_CHECK_WHITE) {
    Prompts::check(WHITE);
    cout << endl;
    _turn++;
    _num_rand_gen++;
  } else if(result == MOVE_CHECK_BLACK) {
    Prompts::check(BLACK);
    cout << endl;
    _turn++;
    _num_rand_gen++;    
  } else if(result == MOVE_CASTLE) { // move was a castle
    cout << endl;
    _turn++;
    _num_rand_gen++;    
  } else if(result == MOVE_CHECKMATE) { // move caused checkmate
    if(player_turn() == WHITE) 
      Prompts::checkmate(BLACK);
    if(player_turn() == BLACK) 
      Prompts::checkmate(WHITE);
    Prompts::win(player_turn(), _turn);
    Prompts::game_over(); 
  } else if(result == MOVE_CHECKMATE_WHITE) { // move caused checkmate 
    Prompts::checkmate(WHITE);
    Prompts::win(BLACK, _turn);
    Prompts::game_over();
  } else if(result == MOVE_CHECKMATE_BLACK) { // move caused checkmate 
    Prompts::checkmate(BLACK);
    Prompts::win(WHITE, _turn);
    Prompts::game_over();
  } else if(result == MOVE_KING_OF_THE_HILL) { // move caused king of the hill victory
    Prompts::conquered(player_turn());
    Prompts::win(player_turn(), _turn);
    Prompts::game_over();    
  } else if(result == MOVE_STALEMATE) { // move caused stalemate
    Prompts::stalemate();
    Prompts::game_over();
  } else if(result == GHOSTED) {
    Prompts::ghost_capture();
    cout << endl;
    _turn++;
    _num_rand_gen++;
  }
}

/* NEED TO "SAVE" TO SAVE GAME TO FILE */
// Execute the main gameplay loop.
void Game::run(int game_choice) {
  Prompts::player_prompt(player_turn(), turn());
  string input; // initialize string to take user input
  bool board = false; // set boolean for if board is being displayed
  getline(cin, input); // get rid of previous output statement
  while(getline(cin, input)) { // loop for user input
    if(input == "q") { // user wants to quit program, ends loop
      Prompts::game_over();
      break;
    } else if (input == "save") {
      Prompts::save_game();
      string filename;
      getline(cin, filename);
      save(filename, game_choice);
      cout << endl;
      Prompts::player_prompt(player_turn(), turn());
    } else if(input == "forfeit") {
      if(player_turn() == 0) {
	Prompts::win(BLACK, turn());
      }
      else if(player_turn() == 1) {
	Prompts::win(WHITE, turn());
      }
      Prompts::game_over();
      break;
    }
    else if(input == "board") {
      if(board == false) {
	board = true;
	cout << endl;
	display_board();
	Prompts::player_prompt(player_turn(), turn());
      }
      else {
	board = false;
	cout << endl;
	Prompts::player_prompt(player_turn(), turn());
      }
    } else {     
      istringstream iss(input); // parse through string for start position
      char coln; // column character
      char row; // x coordinate
      iss >> coln;
      iss >> row;
      
      int result;
      Position start;
      Position end;
      if(!isalpha(coln) || !isdigit(row)) {
	result = PARSE_ERROR;
      } else {
	string num_string = {row};
	int x = stof(num_string); //convert row character to int
	int y = (int)coln; // get int value of char column
	// convert ascii int value to coordinate value
	if(y < 97) // check if uppercase
	  y -= 65;
	else
	  y -= 97;
	x -= 1; // change value to array index value
	Position temp(y, x); // set start position
	start = temp;
	
	iss >> coln;
	iss >> row;
	string check = "";
	iss >> check;
	if(!isalpha(coln) || !isdigit(row) || check != "") {
	  result = PARSE_ERROR;
	}else {
	  string num_string = {row};
	  int x = stof(num_string);
	  int y = (int)coln;
	  if(y < 97)
	    y -= 65;
	  else
	    y -= 97;
	  x -= 1;
	  Position temp(y, x); // set end position
	  end = temp;
	  if((start.x == end.x) && (start.y == end.y))
	    result = MOVE_ERROR_ILLEGAL;
	  else{
	    int i = 0;
	    while(i == 0) {
	      if(game_choice == SPOOKY_CHESS) {
		result = make_move(start, end, game_choice);
		if(result == MOVE_CHECKMATE || result == MOVE_STALEMATE)
		  i = 1;
		else {
		  bool captured = false;
		  bool check = false;
		  if(result == MOVE_CAPTURE)
		    captured = true;
		  if(result == MOVE_CHECK)
		    check = true;
		  if(result > 0)  
		    result = make_ghost_move(result);
		  if(captured == true && result == GHOSTED) {
		    Prompts::capture(player_turn());
		    i = 1;
		  } else if(check == true && result == GHOSTED) {
		    Prompts::check(player_turn());
		    i = 1;
		  } else 
		    i = 1;
		}
	      } else { 
		  result = make_move(start, end, game_choice); // get result from make_move function
		  i = 1;
	      }
	    }
	  }
	}
      }
      // VICTORY CHECK
      get_result(result);
      if(result == MOVE_CHECKMATE || result == MOVE_STALEMATE || result == MOVE_CHECKMATE_WHITE || result == MOVE_CHECKMATE_BLACK || result == MOVE_KING_OF_THE_HILL)
	break;
      if(board == true)
	display_board();
      Prompts::player_prompt(player_turn(), turn());
    }
  }
}

// Search the factories to find a factory that can translate
//`piece_type' into a Piece, and use it to create the Piece.
// Returns nullptr if factory not found.
Piece* Game::new_piece(int piece_type, Player owner) {
    PieceGenMap::iterator it = _registered_factories.find(piece_type);

    if (it == _registered_factories.end()) { // not found
        std::cout << "Piece type " << piece_type << " has no generator\n";
        return nullptr;
    } else {
        return it->second->new_piece(owner);
    }
}



// Add a factory to the Board to enable producing
// a certain type of piece. Returns whether factory
// was successfully added or not.
bool Game::add_factory(AbstractPieceFactory* piece_gen) {
    // Temporary piece to get the ID
    Piece* p = piece_gen->new_piece(WHITE);
    int piece_type = p->piece_type();
    delete p;

    PieceGenMap::iterator it = _registered_factories.find(piece_type);
    if (it == _registered_factories.end()) { // not found, so add it
        _registered_factories[piece_type] = piece_gen;
        return true;
    } else {
        std::cout << "Piece type " << piece_type << " already has a generator\n";
        return false;
    }
}

