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

#include "SpookyChessGame.h"
#include "ChessGame.h"
#include "Game.h"
#include "Prompts.h"
#include "Enumerations.h"

using std::ifstream;
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::ofstream;

// Game variant enumeration
enum GameName {STANDARD_CHESS = 1, KING_OF_THE_HILL, SPOOKY_CHESS};

// Creates new game in spooky chess start-of-game state
SpookyChessGame::SpookyChessGame() {
  add_factory(new PieceFactory<Ghost>(GHOST_ENUM));
  init_piece(GHOST_ENUM, NO_ONE, Position(0, 4));

  _num_rand_gen = 0;
}  

/* Function to get a ghost's position. */
Position SpookyChessGame::get_ghost_position() {
  // loop through each piece to find ghost
  for(int i = 0; i < (int)width(); i++) {
    for(int j = 0; j < (int)height(); j++) {
      Position pos(i, j);
      Piece *p = get_piece(pos);
      // check if piece exists, is a ghost
      if(p != NULL && p->piece_type() == GHOST_ENUM) {
	return pos;
      }
    }
  }
  Position p(0, 0);
  return p;
}

int SpookyChessGame::make_ghost_move(int result) {
  Position start_g = get_ghost_position();
  Piece *ghost = get_piece(start_g);
  Position end_g;
  srand(322);
  for(int i = 0; i < _num_rand_gen; i++){
    rand();
  }
  int r = rand() % 64;
  end_g.y = r / 8;
  end_g.x = r % 8;
  Piece *target = get_piece(end_g);

  while(target != NULL && target->piece_type() == KING_ENUM) {
    int r = rand() % 64;
    end_g.y = r / 8;
    end_g.x = r % 8;
    target = get_piece(end_g);
    _num_rand_gen++;
  }

  while(start_g.x == end_g.x && start_g.y == end_g.y) {
    int r = rand() % 64;
    end_g.y = r / 8;
    end_g.x = r % 8;
    target = get_piece(end_g);
    _num_rand_gen++;
  }

  Position white_king = get_king_position(WHITE);
  Position black_king = get_king_position(BLACK);
  bool wcheck = false;
  bool bcheck = false;
  bool wcheckmate = false;
  bool bcheckmate = false;
  bool ghosted = false;
  if(!(target != NULL && target->piece_type() == KING_ENUM) && !(start_g.x == end_g.x && start_g.y == end_g.y)) {
    if(target == NULL) {
      _pieces[index(end_g)] = ghost;
      _pieces[index(start_g)] = NULL;
    }
    if(target != NULL) {
      delete _pieces[index(end_g)]; // delete piece ghost is capturing
      _pieces[index(end_g)] = ghost; // put ghost at end position
      _pieces[index(start_g)] = NULL; // set original position to NULL
      ghosted = true;
    }
    
    wcheck = check_check(white_king, start_g, start_g, WHITE, start_g, 10000000, true);
    bcheck = check_check(black_king, start_g, start_g, BLACK, start_g, 10000000, true); 
    if((wcheck == true || bcheck == true) && result != MOVE_CHECK) {
      if(wcheck == true && bcheck == false) {
	wcheckmate = check_stalemate(start_g, WHITE);
	if(wcheckmate == true || player_turn() == WHITE)
	  return MOVE_CHECKMATE_WHITE;
	else {
	  return MOVE_CHECK_WHITE;
	}
      }else if(bcheck == true && wcheck == false) {
	bcheckmate = check_stalemate(start_g, BLACK);
	if(bcheckmate == true || player_turn() == BLACK)
	  return MOVE_CHECKMATE_BLACK;
	else
	  return MOVE_CHECK_BLACK;
      }
    } else {
      bool stalemate_white = check_stalemate(start_g, WHITE); // check if no possible moves available for WHITE
      bool stalemate_black = check_stalemate(start_g, BLACK); // check if no possible moves available for BLACK
      if(stalemate_white == true || stalemate_black == true) // if no moves possible, return stalemate value
	return MOVE_STALEMATE;
    }
  }
  if(ghosted == true)
    return GHOSTED;
  return result;
}
