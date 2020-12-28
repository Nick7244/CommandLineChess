/* Derek Chang dchang38 dchang38@jhu.edu
 * Nick Maritato nmarita1 nmarita1@jhu.edu
 * Yunfei Li yli208 yli208@jhu.edu
 * 601.220
 * Final Project
 * 12/08/2017
 */

#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <iostream>
#include <cmath>
#include <random>
#include "Enumerations.h"
#include "Piece.h"
#include "Game.h"
#include "ChessGame.h"

using std::cout;
using std::endl;

class Pawn : public Piece {

protected:
    friend PieceFactory<Pawn>;
    Pawn(Player owner, int piece_type) : Piece(owner, piece_type) {}

public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override {
      int x_diff = abs(end.x - start.x); // get coln difference
      int y_diff = end.y - start.y; // get row difference
      if(start.x == end.x) { // check for regular move forward
	if(y_diff == 2 && start.y == 1 && owner() == WHITE) { // if pawn is white and moves 2 spaces from initial position
	  int temp = start.y + 1; // get first position
	  Position p(start.x, temp); // initialize position object
	  // add each position space to vector
	  trajectory.push_back(start);
	  trajectory.push_back(p);
	  trajectory.push_back(end);
	  return SUCCESS;
	} else if(y_diff == -2 && start.y == 6 && owner() == BLACK) { // if pawn is black and moves 2 spaces from initial position
	  int temp = start.y - 1;
	  Position p(start.x, temp);
	  trajectory.push_back(start);
	  trajectory.push_back(p);
	  trajectory.push_back(end);
	  return SUCCESS;
	}else if((y_diff == 1 && owner() == WHITE) || (y_diff == -1 && owner() == BLACK)) { // pawn is moving forward in regular movement
	  return SUCCESS;
	}
      } else if(x_diff == 1) { // pawn is attacking another piece
	if((y_diff == 1 && owner() == WHITE) || (y_diff == -1 && owner() == BLACK)) { // pawn moves diagonally one space
	  return MOVE_CAPTURE;
	}
      }
      return -1;
    }  
};


class Rook : public Piece {

protected:
    friend PieceFactory<Rook>;
    Rook(Player owner, int piece_type) : Piece(owner, piece_type) {}

public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override {
      int x_diff = end.x - start.x; // get coln difference
      int y_diff = end.y - start.y; // get row difference
      trajectory.push_back(start);
      int diff = 1; // initialize increment value
      if(x_diff == 0 && y_diff != 0) { // piece moves horizontally
	if(y_diff < 0) { // if piece moves left
	  diff = -1;
	}
	int temp = start.y + diff; // get coordinate of next position
	for(int i = 1; i < abs(y_diff); i++) { // add each position to vector
	  Position p(start.x, temp);
	  trajectory.push_back(p);
	  temp += diff;
	}
	trajectory.push_back(end);
	return SUCCESS;
      } else if(y_diff == 0 && x_diff != 0) { // piece moves vertically
	if(x_diff < 0) { // piece moves down
	  diff = -1;
	}
	int temp = start.x + diff;
	for(int i = 1; i < abs(x_diff); i++) {
	  Position p(temp, start.y);
          trajectory.push_back(p);
	  temp += diff;
        }
	trajectory.push_back(end);
	return SUCCESS;
      }
      return -1;
    }
};



class Knight : public Piece {

protected:
    friend PieceFactory<Knight>;
    Knight(Player owner, int piece_type) : Piece(owner, piece_type) {}

public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override {
      int x_diff = abs(end.x - start.x); // get coln difference
      int y_diff = abs(end.y - start.y); // get row difference
      trajectory.push_back(start); // get initial position
      if(x_diff == 1 && y_diff == 2) { // moves in vertical L-shape
	int diff = 1; // set value to positive for move upwards
	int temp = end.y - start.y;
        if(temp < 0) // knight moves down, change to negative
	  diff = -1;
        temp = start.y + diff;
	for(int i = 0; i < y_diff; i++) { // get 2 vertical positions
	  Position p(start.x, temp);
	  trajectory.push_back(p);
	  temp += diff;
	}
	trajectory.push_back(end);
	return SUCCESS;
      } else if(x_diff == 2 && y_diff == 1) { // moves in horizontal L-shape
	int diff = 1; // set value to positive for move to the right
	int temp = end.x - start.x;
	if(temp < 0) // knight moves left, change to negative
	  diff = -1;
	temp = start.x + diff;
	for(int i = 0; i < x_diff; i++) { // get 2 horizontal positions
	  Position p(temp, start.y);
	  trajectory.push_back(p);
	  temp += diff;
	}
	trajectory.push_back(end);
	return SUCCESS;
      }
      return -1;
    }
};


class Bishop : public Piece {

protected:
    friend PieceFactory<Bishop>;
    Bishop(Player owner, int piece_type) : Piece(owner, piece_type) {}

public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override {
      int x_diff = end.x - start.x; // get coln difference
      int y_diff = end.y - start.y; // get row difference
      trajectory.push_back(start); // get initial position
      if(abs(x_diff) == abs(y_diff) && x_diff != 0) { // check bishop moves diagonally
	int diffx = 1; // initialize increment value
	int diffy = 1;
	if(x_diff > 0 && y_diff < 0) { // bishop moves southeast
	  diffy = -1;
	} else if(x_diff < 0 && y_diff > 0) { // bishop moves northwest
	  diffx = -1;
	} else if(x_diff < 0 && y_diff < 0) { // bishop moves southwest
	  diffx = -1;
	  diffy = -1;
	}
	// get first position coordinates
	int temp_x = start.x + diffx;
	int temp_y = start.y + diffy;
	x_diff = abs(x_diff);
	for(int i = 1; i < x_diff; i++) { // loop through each position
	  Position p(temp_x, temp_y);
	  trajectory.push_back(p);
	  // increment each value
	  temp_x += diffx;
	  temp_y += diffy;
	}
	trajectory.push_back(end);
	return SUCCESS;
      }
      return -1;
    }
};


class Queen : public Piece {

protected:
    friend PieceFactory<Queen>;
    Queen(Player owner, int piece_type) : Piece(owner, piece_type) {}

public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override {
      int x_diff = end.x - start.x; // get coln difference
      int y_diff = end.y - start.y; // get row difference
      trajectory.push_back(start); // get initial position
      int diffx = 1; // initialize increment value
      int diffy = 1;
      int max; // number of iterations
      // check for diagonal, horizontal, and vertical movements
      if(abs(x_diff) == abs(y_diff) && x_diff != 0) { // queen moves diagonally
        if(x_diff > 0 && y_diff < 0) { // queen moves southeast
          diffy = -1;
        } else if(x_diff < 0 && y_diff > 0) { // queen moves northwest
          diffx = -1;
        } else if(x_diff < 0 && y_diff < 0) { // queen moves southwest
          diffx = -1;
          diffy = -1;
        }
	int temp_x = start.x + diffx;
	int temp_y = start.y + diffy;
	max = abs(x_diff); // set max number of iterations
	for(int i = 1; i < max; i++) {
	  Position p(temp_x, temp_y);
	  trajectory.push_back(p);
	  temp_x += diffx;
	  temp_y += diffy;
	}
	trajectory.push_back(end);
        return SUCCESS;
      } else if(x_diff == 0 && y_diff != 0) { // queen moves horizontally
	if(y_diff < 0) // queen moves to the left
	  diffy = -1;
	int temp = start.y + diffy;
	max = abs(y_diff);
	for(int i = 1; i < max; i++) {
	  Position p(start.x, temp);
	  trajectory.push_back(p);
	  temp += diffy;
	}
	trajectory.push_back(end);
        return SUCCESS;
      } else if(y_diff == 0 && x_diff != 0) { // queen moves vertically
	if(x_diff < 0) // queen moves down
	  diffx = -1;
	int temp = start.x + diffx;
	max = abs(x_diff);
	for(int i = 1; i < max; i++) {
          Position p(temp, start.y);
          trajectory.push_back(p);
          temp += diffx;
        }
	trajectory.push_back(end);
	return SUCCESS;
      }
      return -1;
    }
};


class King : public Piece {

protected:
    friend PieceFactory<King>;
    King(Player owner, int piece_type) : Piece(owner, piece_type) {}

public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override {
      int x_diff = end.x - start.x; // get coln difference
      int y_diff = end.y - start.y; // get row difference
      trajectory.push_back(start); // get initial position
      int diffx = 1; // initialize increment value
      // check for diagonal, horizontal, and vertical movements
      if(abs(x_diff) == 1 && abs(y_diff) == 1) { // king moves diagonally
        trajectory.push_back(end);	  
        return SUCCESS;
      } else if(x_diff == 0 && abs(y_diff) == 1) { // king moves horizontally
        trajectory.push_back(end);
        return SUCCESS;
      } else if(y_diff == 0 && abs(x_diff) == 1) { // king moves vertically
        trajectory.push_back(end);
        return SUCCESS;
      } else if(abs(x_diff) == 2 && y_diff == 0) { //king castle
	if(x_diff < 0)
	  diffx = -1;
	int temp = start.x + diffx;
	Position p(temp, start.y);
	trajectory.push_back(p);
	trajectory.push_back(end);
	return MOVE_CASTLE;
      }
      return -1;
    }
};

class Ghost : public Piece {

protected:
    friend PieceFactory<Ghost>;
    Ghost(Player owner, int piece_type) : Piece(owner, piece_type) {}
    
public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override {
      trajectory.push_back(start); // get initial position
      srand(322);
      int r = rand() % 64;
      end.x = r / 8;
      end.y = r % 8;
      //Position white_king = get_king_position(WHITE); // get the white king's position
      //Position black_king = get_king_position(BLACK); // get the black king's position
      
      Position p(end.x, end.y);
      //if((p.x == white_king.x && p.y == white_king.y) || (p.x == black_king.x && p.y == black_king.y) {
      trajectory.push_back(p);
      trajectory.push_back(end);
      return -1;
}
    
};

#endif // CHESS_PIECE_H
