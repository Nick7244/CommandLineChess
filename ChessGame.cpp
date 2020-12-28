/* Derek Chang dchang38 dchang38@jhu.edu
 * Nick Maritato nmarita1 nmarita1@jhu.edu
 * Yunfei Li yli208 yli208@jhu.edu
 * 601.220
 * Final Project
 * 12/08/2017
 */

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include "Piece.h"
#include "Game.h"
#include "ChessGame.h"
#include "Prompts.h"
#include "Enumerations.h"

using std::ifstream;
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::ofstream;

static bool castle_valid_black_right = true;
static bool castle_valid_black_left = true;
static bool castle_valid_white_right = true;
static bool castle_valid_white_left = true;

// Game variant enumeration
enum GameName {STANDARD_CHESS = 1, KING_OF_THE_HILL, SPOOKY_CHESS};

// Check which column a piece is in
int ChessGame::check_coln(char c) {
  if(c == 'a') {
    return 0;
  } else if(c == 'b') {
    return 1;
  } else if(c == 'c') {
    return 2;
  } else if(c == 'd') {
    return 3;
  } else if(c == 'e') {
    return 4;
  } else if(c == 'f') {
    return 5;
  } else if(c == 'g') {
    return 6;
  } else if(c == 'h') {
    return 7;
  } else {
    return 0;
  }
}

// Convert to column
char ChessGame::convert_coln(int i) {
  if(i == 0) {
    return 'a';
  } else if(i == 1) {
    return 'b';
  } else if(i == 2) {
    return 'c';
  } else if(i == 3) {
    return 'd';
  } else if(i == 4) {
    return 'e';
  } else if(i == 5) {
    return 'f';
  } else if(i == 6) {
    return 'g';
  } else if(i == 7) {
    return 'h';
  } else {
    return 0;
  }
}


// Set up the chess board with standard initial pieces
ChessGame::ChessGame(): Game() {
    initialize_factories();
    std::vector<int> pieces {
        ROOK_ENUM, KNIGHT_ENUM, BISHOP_ENUM, QUEEN_ENUM,
	  KING_ENUM, BISHOP_ENUM, KNIGHT_ENUM, ROOK_ENUM
    };
    for (size_t i = 0; i < pieces.size(); ++i) {
        init_piece(PAWN_ENUM, WHITE, Position(i, 1));
        init_piece(pieces[i], WHITE, Position(i, 0));
        init_piece(pieces[i], BLACK, Position(i, 7));
        init_piece(PAWN_ENUM, BLACK, Position(i, 6));
    }
}

// Set up the chess board with game state loaded from file
ChessGame::ChessGame(const std::string filename, int game_choice, bool& run_game) : Game() {
  initialize_factories(); // set up board
  
  string type; // type of chess game
  int color; // color of the piece
  char col; // coln of the pieces
  int x, y; // coordinates of the pieces
  int piece_type; // type of piece
  ifstream infile;
  infile.open(filename); // open file
  
  if(!infile.is_open()) { // file fails to read
    Prompts::load_failure();
    run_game = false;
    infile.close(); //close file
  } else if(infile.peek() == std::ifstream::traits_type::eof()) { // checks if file is empty
    Prompts::load_failure();
    run_game = false;
    infile.close(); //close file
  } else {
    infile >> type; // get type of game
    if(type == "") { // fails to read in file
      Prompts::load_failure();
      run_game = false;
      infile.close(); //close file
    } else if((game_choice == STANDARD_CHESS && type != "chess") // invalid chess game
       || (game_choice == KING_OF_THE_HILL && type != "king") // invalid king of the hill game
       || (game_choice == SPOOKY_CHESS && type != "spooky")) { // invalid spooky chess game
      Prompts::load_failure();
      run_game = false;
      infile.close(); //close file
    } else if (type == "chess" || type == "king" || type == "spooky") {
      infile >> _turn; //get turn number
      int i = 0;      
      while(infile.good()) { // loop while file has information
	if(i == 0) {
	  if (type == "spooky") { // game is spooky
	    add_factory(new PieceFactory<Ghost>(GHOST_ENUM));
	    infile >> _num_rand_gen;
	    Position end_g = Position(0, 4);
	    srand(322);
	    if(_num_rand_gen > 0) {
	      for(int i = 0; i < _num_rand_gen -1; i++){
		rand();
	      } 
	      int r = rand() % 64;
	      end_g.y = r / 8;
	      end_g.x = r % 8;
	    }
	    init_piece(GHOST_ENUM, NO_ONE, end_g);
	  }
	}
	string check; // color of piece
	infile >> check;
	if(check == ""){ //checks if there is an empty line in the file
	  continue;
	} else {
	  if(check == "0")
	    color = 0;
	  else if(check == "1")
	    color = 1;
	  infile >> col; // get coln
	  x = check_coln(col); // convert to int
	  infile >> y; // get row number
	  y--;
	  infile >> piece_type; // get piece type
	  
	  Position coordinates(x, y);
	  Player owner;
	  if(color == 0)
	    owner = WHITE;
	  else
	    owner = BLACK;
	  init_piece(piece_type, owner, coordinates); //initialized loaded in pieces
	}
	i = 2;
      }
      infile.close(); //close file
    } else {
      Prompts::load_failure();
      run_game = false;
      infile.close(); //close file
    }
  }
}

/* Function to check if a piece with the given trajectory and move type is blocked. */
bool ChessGame::check_blocked(const Piece *piece, const vector<Position> &trajectory, const int type_move, Player player) {
  if(piece->piece_type() != KNIGHT_ENUM) { // check piece isn't knight
    for(int i = 1; i < (int)trajectory.size(); i++) { // check for pieces in trajectory
      Position step = trajectory[i];
      Piece *p = get_piece(step);
      if(p != NULL) { // piece exists at location
	// check if p isn't at end position and p is other player's piece
	if(i != (int)(trajectory.size() - 1) && p->owner() != player)
	  return true;
	
	// check if piece is a pawn and isn't attacking (i.e. moving diagonally)
	if(piece->piece_type() == PAWN_ENUM && type_move != MOVE_CAPTURE)
	  return true;
	  
	// check if own player's piece is in trajectory
	if(p->owner() == player)
	  return true;
      }
    }
  }
  return false;
}

/* Function to get a kings position of a specific player. */
Position ChessGame::get_king_position(Player player) {
  // loop through each piece to find king of player
  for(int i = 0; i < (int)width(); i++) {
    for(int j = 0; j < (int)height(); j++) {
      Position pos(i, j);
      Piece *p = get_piece(pos);
      // check if piece exists, is a king, and is owned by the player
      if(p != NULL && p->piece_type() == KING_ENUM && p->owner() == player) {
	return pos;
      }
    }
  }
  Position p(0, 0);
  return p;
}

/* Function to get a kings position of a specific player ignoring where the king started from. */
Position ChessGame::get_king_position(Player player, Position exempt) {
  // loop through each piece to find king of player
  for(int i = 0; i < (int)width(); i++) {
    for(int j = 0; j < (int)height(); j++) {
      Position pos(i, j);
      if(pos.x != exempt.x || pos.y != exempt.y) { // check king isn't at position where it moved from
	Piece *p = get_piece(pos);
	// check if piece exists, is a king, and is owned by the player
	if(p != NULL && p->piece_type() == KING_ENUM && p->owner() == player)
	  if(pos.x != exempt.x || pos.y != exempt.y) // check king isn't at position where it moved from
	    return pos;
      }
    }
  }
  Position p(0, 0);
  return p;
}

bool ChessGame::edit_trajectory(const Piece* p, vector<Position>& trajectory, Position pos, Position exempt1, Position exempt2, int success) {
  Piece* target = get_piece(pos);
  if(success > 0 &&
     !(target == NULL && success == MOVE_CAPTURE && p->piece_type() == PAWN_ENUM) &&
     !(target != NULL && success == MOVE_CASTLE)
     && !(target != NULL && target->piece_type() == GHOST_ENUM)
     && !(target != NULL && target->owner() == p->owner())
     && !(target != NULL && success == SUCCESS && p->piece_type() == PAWN_ENUM)) { // piece can be moved successfully
    for(int k = 0; k < (int)trajectory.size(); k++) {
      Position temp = trajectory[k];
      if((temp.x == exempt1.x && temp.y == exempt1.y) || (temp.x == exempt2.x && temp.y == exempt2.y)) { // check if position is position to ignore
	trajectory.erase(trajectory.begin() + k);
	break;
      }
    }
    return true; // trajectory edited successfully
  } else
    return false; // trajectory not able to be edited
}

/* Function to check if a king is in check with the current board, exempts spot where piece was moved from. */
bool ChessGame::check_check(Position king, Position exempt1, Position exempt2, Player player, Position end, int result, bool checking_for_self) {
  Piece *temp;
  Piece *capture;
  Piece *attacker;
  if(result == MOVE_CAPTURE) { //temporarily makes the capture 
    temp = new_piece(0, NO_ONE);
    capture = get_piece(end);
    attacker = get_piece(exempt1);
    temp->change_owner(capture->owner());
    temp->change_piece_type(capture->piece_type());
    capture->change_owner(attacker->owner());
    capture->change_piece_type(attacker->piece_type());
    if(capture->piece_type() == KING_ENUM) {//if king attacked
      king = get_king_position(player, exempt1); //gets new king position in case king captures
    }
  }
    
 for(int i = 0; i < (int)width(); i++) {
    for(int j = 0; j < (int)height(); j++) {
      Position pos(i, j);
      Piece *p = get_piece(pos);
      // check piece isn't at exempt position, exists and is on other team
      if((pos.x != exempt1.x || pos.y != exempt1.y) && (pos.x != exempt2.x || pos.y != exempt2.y)
	 && p != NULL && p->owner() != player) {
	vector<Position> trajectory;
	int success;
	if(result == MOVE_CAPTURE && checking_for_self == false) { // piece is attacking another
	  if(p->owner() == attacker->owner() && p->owner() != player // piece not same color as player being checked
	     && p->piece_type() == attacker->piece_type()) { // piece matches attacker type
	    success = attacker->valid_move_shape(pos, king, trajectory); //checks if attacking piece is in a position to capture the opponents king
	  } else {
	    success = -1;
	  }
	}else 
	  success = p->valid_move_shape(pos, king, trajectory);
	// edit trajectory vector to get rid of exempt positions	
        bool traj = edit_trajectory(p, trajectory, king, exempt1, exempt2, success);
	if(traj == true) {
	  bool blocked = check_blocked(p, trajectory, success, p->owner());
	  if(blocked == false) { // piece isn't blocked to king's position
	    if(result == MOVE_CAPTURE) { //reverses capture
	      capture->change_owner(temp->owner());
	      capture->change_piece_type(temp->piece_type());
	      delete temp;
	    }
	    return true;
	  }
	}
      }
    }
  }
  if(result == MOVE_CAPTURE) { //reverses capture
    capture->change_owner(temp->owner());
    capture->change_piece_type(temp->piece_type());
    delete temp;
  }
  return false;
}

/* Function to check if a piece can't move. */
bool ChessGame::check_stale(Position pos, Position exempt, Player player) {
  Piece *piece = get_piece(pos); // piece pointer to piece
  // check each position on board for possible move for piece
  if(piece != NULL) { // check piece exists
    for(int i = 0; i < (int)width(); i++) {
      for(int j = 0; j < (int)height(); j++) {
	vector<Position> trajectory; // trajectory of piece
	Position temp(i, j);
	int success = piece->valid_move_shape(pos, temp, trajectory);
	Piece* target = get_piece(temp); // piece at position being moved to
        bool traj = edit_trajectory(piece, trajectory, temp, exempt, exempt, success);
	if(traj == true) {
	  bool blocked = check_blocked(piece, trajectory, success, player);
	  if(blocked == false) { // move isn't blocked
	    int result = 0;
	    bool changed = false;
	    if(target == NULL && success == SUCCESS) { //piece is moving to an open spot
	      init_piece(piece->piece_type(), player, temp);
	      changed = true;
	      result = SUCCESS;
	    } else if(target != NULL && target->owner() != player) { // piece is attacking another piece
	      result = MOVE_CAPTURE;
	    }
	    if(result > 0) {
	      Position king = get_king_position(player);
	      if(piece->piece_type() == KING_ENUM)
		king = get_king_position(player, pos);
	      bool check;
	      check = check_check(king, pos, exempt, player, temp, result, true); // check if moving piece puts king in check
	      if(changed == true) {
		delete _pieces[index(temp)];
		_pieces[index(temp)] = NULL;
	      } if(check == false)  // move is valid and doesn't put king in check
		return false; // a move is possible
	    }else {
	      if(changed == true) {
		delete _pieces[index(temp)];
		_pieces[index(temp)] = NULL;
	      }
	    }
	  }
	}
      }
    }
  }
  return true; // no viable move found
}

/* Function to check if player is in stalemate. */
bool ChessGame::check_stalemate(Position exempt, Player player) {
  for(int i = 0; i < (int)width(); i++) {
    for(int j = 0; j < (int)height(); j++) {
      Position temp(i, j);
      Piece *p = get_piece(temp);
      if(p != NULL && p->owner() == player && !(temp.x == exempt.x && temp.y == exempt.y)) { // check piece exists and is owned by player and is not at exempt position
	bool stalemate = check_stale(temp, exempt, player); //check is piece has valid moves
	if(stalemate == false) // piece with valid move found
	  return false;
      }
    }
  }
  return true;
}

/* Function to complete a castle move. */
Position ChessGame::castle(Position end) {
  Position rook; // position for the rook that's castling
  Position cas; // position rook is moving to for castle
  if(end.x > 4) { // king is moving right
    rook.x = 7; // x coordinate of the white rook
    cas.x = end.x - 1; // x coordinate of the white rook's end position
  } else { // king is moving left
    rook.x = 0;
    cas.x = end.x + 1;
  }
  if(player_turn() == WHITE) { // castle for white team
    rook.y = 0; // y coordinate of the white rook
    cas.y = 0; // y coordinate of the white rook's end position
  } else if(player_turn() == BLACK) { // castle still valid for black team
    rook.y = 7; // y coordinate of the black rook
    cas.y = 7; // y coordinate of the black rook's end position
  }
  init_piece(ROOK_ENUM, player_turn(), cas); // make new rook at new position
  return rook; // position of rook before move
}

Position ChessGame::check_castle(Position end, Position king, Position temp) {
  Position result(5, 5); // position of rook at start, (5, 5) if not valid
  if(end.x > 4) { // castle to the right
    // check castle is valid for player
    Position wRook_pos(7, 0);
    Position bRook_pos(7, 7);
    Piece *wRook = get_piece(wRook_pos); //piece to check for position of white rook
    Piece *bRook = get_piece(bRook_pos); //piece to check for position of black rook
    // check castle doesn't move through check position
    int test = end.x - 1;
    Position move(test, end.y);
    bool through_check = check_check(move, end, king, player_turn(), end, MOVE_CASTLE, true);
    if(((player_turn() == WHITE && castle_valid_white_right == true && king.x == 4 && king.y == 0 &&
	wRook != NULL && wRook->owner() == WHITE && wRook->piece_type() == 1) // checks for white valid castle
       || (player_turn() == BLACK && castle_valid_black_right == true && king.x == 4 && king.y == 7 &&
	   bRook != NULL && bRook->owner() == BLACK && bRook->piece_type() == 1)) // checks for black valid castle
       && through_check == false) // castle doesn't move through check position
      result = castle(end); // complete castle move
    else { //invalid castle occurs
      _pieces[index(end)] = NULL; //sets new king position to null since it was initialized before this
      return temp;
    }
  } else { // castle to the left
    Position wRook_pos(0, 0);
    Position bRook_pos(0, 7);
    Position wcheck_pos(1, 0);
    Position bcheck_pos(1, 7);
    Piece *wRook = get_piece(wRook_pos); // piece to check for position of white rook
    Piece *bRook = get_piece(bRook_pos); // piece to check for position of black rook
    Piece *wcheck = get_piece(wcheck_pos); // piece to check if 3rd space between white king and rook is empty
    Piece *bcheck = get_piece(bcheck_pos); // piece to check if 3rd space between black king and rook is empty
    // check castle doesn't move through check position
    int test = end.x + 1;
    Position move(test, end.y);
    bool through_check = check_check(move, end, king, player_turn(), end, MOVE_CASTLE, true);
    if(((player_turn() == WHITE && castle_valid_white_left == true && king.x == 4 && king.y == 0 &&
	wRook != NULL && wRook->owner() == WHITE && wRook->piece_type() == 1 && wcheck == NULL) // checks for white valid castle
       || (player_turn() == BLACK && castle_valid_black_left == true && king.x == 4 && king.y == 7 &&
	   bRook != NULL && bRook->owner() == BLACK && bRook->piece_type() == 1 && bcheck == NULL)) // checks for black valid castle
       && through_check == false) // castle doesn't move through check position
      result = castle(end); // complete castle move
    else {// invalid castle occurs
      if((player_turn() == WHITE && wcheck != NULL) || (player_turn() == BLACK && bcheck != NULL))
	return result;
      else
	return temp;
    }
  }
  return result; // returns new position of rook
}

// check if castle in a certain direction is now invalid
void ChessGame::castle_valid(Piece *piece, Position start) {
  if(piece->piece_type() == ROOK_ENUM || piece->piece_type() == KING_ENUM) { // if piece is king or rook set valid castle to false
    if(start.x == 0 || player_turn() == WHITE) // white rook on left or king moves
      castle_valid_white_left = false;
    if(start.x == 7 || player_turn() == WHITE) // white rook on right or king moves
      castle_valid_white_right = false;
    if(start.x == 0 || player_turn() == BLACK) // black rook on left or king moves
      castle_valid_black_left = false;
    if(start.x == 7 || player_turn() == BLACK) // black rook on right or king moves
      castle_valid_black_right = false;
  }
}

// check move is valid and piece is not a pawn attacking an empty space
bool ChessGame::check_move_valid(Piece* target, Piece* piece, int type_move) {
  if(!(target == NULL && type_move == MOVE_CAPTURE && piece->piece_type() == PAWN_ENUM) // piece isn't pawn attacking empty space
     && !(target != NULL && type_move == MOVE_CASTLE) // castling isn't blocked
     && type_move > 0 // move is valid enum
     && !(target != NULL && target->piece_type() == GHOST_ENUM) // cannot attack ghost piece in spooky chess
     && !(target != NULL && target->owner() == player_turn()) // moving to own piece
     && !(target != NULL && type_move == SUCCESS && piece->piece_type() == PAWN_ENUM)) { // piece moving to isn't same type  
    return true; // move is valid
  } else
    return false; // move isn't valid
}

int ChessGame::check_move_check(int type, bool cur, Piece* piece, Position king, Position start, Position end, Position temp, int type_move, int game_type) {
  int result = type;
  bool check = check_check(king, start, temp, player_turn(), end, result, true); // boolean for if move opens player's king to check
  if((cur == false || check == false) && result > 0) { // player's king not currently in check or move cancels check
    if(check == false) { // move doesn't expose player's own king
      if(game_type == KING_OF_THE_HILL) {
	if(piece->piece_type() == KING_ENUM) {
	  int check_win = check_win_cond(end, start, result);
	  if(check_win == MOVE_KING_OF_THE_HILL)
	    return check_win;
	}
      }
      castle_valid(piece, start); // check if castle in a direction isn't valid anymore
      
      // check opponent's king for check
      bool checkmate = false; // boolean for if a player is in checkmate
      if(player_turn() == WHITE) {
	king = get_king_position(BLACK); // get position of opponent king
	check = check_check(king, start, start, BLACK, end, result, false);
      } else if(player_turn() == BLACK) {
	king = get_king_position(WHITE); // get position of opponent king
	check = check_check(king, start, start, WHITE, end, result, false);
      }
      if(check == true) {
	if(result == MOVE_CAPTURE) {
	  Piece *p = get_piece(start);
	  delete _pieces[index(end)];
	  _pieces[index(end)] = p; // set end piece equal to attacking piece
	  _pieces[index(start)] = NULL; // set start position to NULL, gets rid of original piece
	}
	if(result == SUCCESS) {
	  delete _pieces[index(start)];
	  _pieces[index(start)] = NULL; // set start position to NULL, gets rid of original piece
	}
	if (result == MOVE_CASTLE) {
	  delete _pieces[index(start)];
	  _pieces[index(start)] = NULL;
	  delete _pieces[index(temp)];
	  _pieces[index(temp)] = NULL;
	}
	if(player_turn() == WHITE) {
	  checkmate = check_stalemate(start, BLACK);
	} else if(player_turn() == BLACK) {
	  checkmate = check_stalemate(start, WHITE);
	}
	if(checkmate == true)
	  return MOVE_CHECKMATE;
	piece = get_piece(end);
	if(piece->piece_type() == PAWN_ENUM
	   && ((piece->owner() == WHITE && end.y == 7) || (piece->owner() == BLACK && end.y == 0))) { // check if pawn made it to other end of board
	  delete _pieces[index(end)];
	  _pieces[index(end)] = NULL; // deletes pawn
	  init_piece(4, player_turn(), end); // puts queen at pawn position
	}
	return MOVE_CHECK;
      } else {
	if(result == MOVE_CAPTURE) {
	  Piece *p = get_piece(start);
	  delete _pieces[index(end)];
	  _pieces[index(end)] = p; // set end piece equal to attacking piece
	  _pieces[index(start)] = NULL; // set start position to NULL, gets rid of original piece
	}
	if(result == SUCCESS) {
	  delete _pieces[index(start)];
	  _pieces[index(start)] = NULL; // set start position to NULL, gets rid of original piece
	}
	if (result == MOVE_CASTLE) {
	  delete _pieces[index(start)];
	  _pieces[index(start)] = NULL;
	  delete _pieces[index(temp)];
	  _pieces[index(temp)] = NULL;
	}
	bool stalemate_white = check_stalemate(start, WHITE); // check if no possible moves available for WHITE
	bool stalemate_black = check_stalemate(start, BLACK); // check if no possible moves available for BLACK
	if(stalemate_white == true || stalemate_black == true) // if no moves possible, return stalemate value
	  return MOVE_STALEMATE;
	piece = get_piece(end);
	if(piece->piece_type() == PAWN_ENUM
	   && ((piece->owner() == WHITE && end.y == 7) || (piece->owner() == BLACK && end.y == 0))) { // check if pawn made it to other end of board
	  delete _pieces[index(end)];
	  _pieces[index(end)] = NULL; //deletes pawn
	  init_piece(4, player_turn(), end); //puts queen at pawn position
	}
	return result;
      }
    } else {
      if(type_move == MOVE_CASTLE) // move was a valid castle but exposed to check
	return MOVE_ERROR_CANT_CASTLE;
      if(result == SUCCESS) {
	delete _pieces[index(end)];
	_pieces[index(end)] = NULL;
      }
      return MOVE_ERROR_CANT_EXPOSE_CHECK; // else return expose check value
    }
  }
  return result;
}
  /* Function to check type of move and result of move. */
int ChessGame::check_move(bool cur, Piece* piece, Piece* target, int type_move, Position king, Position start, Position end, int game_type) {
  int result = 0; // type of move
  Position pos = king;
  if(target == NULL && (type_move == SUCCESS || type_move == MOVE_CASTLE)) { // check no piece exists at end position and move is not attacking
    init_piece(piece->piece_type(), player_turn(), end);
    result = SUCCESS; // move is regular move
  } else if(target != NULL && target->owner() != player_turn())  // piece is attacking another piece
    result = MOVE_CAPTURE; // move is move capture
  Position temp = start; // position of rook after move if castling, otherwise is start position
  if(piece->piece_type() == KING_ENUM) { // king moved
    if(result != MOVE_CAPTURE)
      pos = get_king_position(player_turn(), start); // get new position of king
    if(type_move == MOVE_CASTLE) { // king is trying to castle
      Position rook = check_castle(end, start, temp); // position of rook
      if(rook.x == temp.x && rook.y == temp.y) { // rook position equals temp, castle is invalid
	delete _pieces[index(end)];
	_pieces[index(end)] = NULL; //sets new king position to null since it was initialized before this
	return MOVE_ERROR_CANT_CASTLE;
      } else if(rook.x == 5) { // castle was blocked
	delete _pieces[index(end)];
	_pieces[index(end)] = NULL; //sets new king position to null since it was initialized before this
	return MOVE_ERROR_BLOCKED;
      }
      // else castle is valid
      temp = rook;
      result = MOVE_CASTLE; // else return value changes to castle
    }
  }
  result = check_move_check(result, cur, piece, pos, start, end, temp, type_move, game_type);
  return result;
}

// Perform a move from the start Position to the end Position                                
// The method returns an integer with the status                                             
// >= 0 is SUCCESS, < 0 is failure                                                           
int ChessGame::make_move(Position start, Position end, int game_type) {
  if(valid_position(start) || valid_position(end)) { // check positions are valid
    Piece *piece = get_piece(start);
    if(piece != NULL && piece->owner() == player_turn()) { // check piece exists and is player's piece
      vector<Position> trajectory; // vector for positions to go through
      int type_move = piece->valid_move_shape(start, end, trajectory);
      Piece *target = get_piece(end); // piece at end location or NULL if empty
      bool valid_move = check_move_valid(target, piece, type_move);
      if(valid_move == true) {
      	bool blocked = check_blocked(piece, trajectory, type_move, player_turn()); // boolean for if piece is blocked
	if(blocked == false) { // piece not blocked
	  int result = 0; // type of move
	  // check own king for current check status
          Position king = get_king_position(player_turn()); // position of player's king
          bool cur = check_check(king, end, end, player_turn(), end, 10000000, true); // boolean for if player's king is currently checked
	  result = check_move(cur, piece, target, type_move, king, start, end, game_type);
	  if(result != SUCCESS || result != MOVE_CAPTURE || result != MOVE_CASTLE) // move was blocked or not valid castle
	    return result;
	  else {
	    if(type_move == MOVE_CASTLE) // move as a valid castle but didn't handle check
	      return MOVE_ERROR_CANT_CASTLE;
	    if(result == SUCCESS) {
	      delete _pieces[index(end)];
	      _pieces[index(end)] = NULL;
	    }
	    return MOVE_ERROR_MUST_HANDLE_CHECK;
	  }
	}else
	  return MOVE_ERROR_BLOCKED;
      } else {
	if((target != NULL && type_move == MOVE_CASTLE) || (target != NULL && target->owner() == player_turn()) || (target != NULL && type_move == SUCCESS && piece->piece_type() == PAWN_ENUM))
	  return MOVE_ERROR_BLOCKED;
	return MOVE_ERROR_ILLEGAL;
      }
    } else
      return MOVE_ERROR_NO_PIECE;
  } else
    return MOVE_ERROR_OUT_OF_BOUNDS;
}

// Save the game
int ChessGame::save(const std::string filename, int game_choice) {
  int king_count = 0;
  vector<std::tuple<Player, char, int, int>> pieces;
  for(unsigned int j = 0; j <  _width; j++) { // loop through each column
    for(unsigned int i = 0; i < _height; i++) { // loop through each row
      Position p(j, i); // get position
      Piece *pi = get_piece(p); // get piece in position
      if(pi != nullptr) { // empty space if no piece at position
	int type = pi->piece_type(); //get piece type
	if(type != GHOST_ENUM) {
	  if(type == KING_ENUM) {
	    king_count++;
	  }
	  Player owner = pi->owner();
	  std::tuple<Player, char, int, int> piece (owner, convert_coln(j), i+1, type);
	  pieces.push_back(piece);
	}
      }
    }
  }    
  if(king_count == 2) {
    ofstream outfile;
    outfile.open(filename); // open file
    if(!outfile.is_open()) {
      Prompts::save_failure();
      return 1;
    }
    if(game_choice == STANDARD_CHESS) 
      outfile << "chess" << endl; // get type of game
    else if(game_choice == KING_OF_THE_HILL) 
      outfile << "king" << endl;
    else if(game_choice == SPOOKY_CHESS) 
      outfile << "spooky" << endl;
    outfile << _turn << endl; // get turn numbers
    if(game_choice == SPOOKY_CHESS)
      outfile << _num_rand_gen << endl;
    for(unsigned int i = 0; i < pieces.size(); i++)
      outfile << std::get<0>(pieces[i]) << " " << std::get<1>(pieces[i]) << std::get<2>(pieces[i]) << " " << std::get<3>(pieces[i]) << endl;
    
    outfile.close(); //close file
    return 0;
  } else {
    Prompts::save_failure();
    return 1;
  }
}

// Report whether the chess game is over                                                    
bool ChessGame::game_over() const {
  return false; 
}

// Prepare the game to create pieces to put on the board
void ChessGame::initialize_factories() {

    // Add all factories needed to create Piece subclasses
    add_factory(new PieceFactory<Pawn>(PAWN_ENUM));
    add_factory(new PieceFactory<Rook>(ROOK_ENUM));
    add_factory(new PieceFactory<Knight>(KNIGHT_ENUM));
    add_factory(new PieceFactory<Bishop>(BISHOP_ENUM));
    add_factory(new PieceFactory<Queen>(QUEEN_ENUM));
    add_factory(new PieceFactory<King>(KING_ENUM));
}
