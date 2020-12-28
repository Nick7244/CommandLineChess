/* Derek Chang dchang38 dchang38@jhu.edu
 * Nick Maritato nmarita1 nmarita1@jhu.edu
 * Yunfei Li yli208 yli208@jhu.edu
 * 601.220
 * Final Project
 * 12/08/2017
 */

#ifndef SPOOKY_CHESS_GAME_H
#define SPOOKY_CHESS_GAME_H

#include <random>
#include "ChessGame.h"

class SpookyChessGame : public ChessGame {

 public:
  // Creates new game in spooky chess start-of-game state
  SpookyChessGame();
  
 SpookyChessGame(std::string filename, int game_choice, bool& run_game) : ChessGame(filename, game_choice, run_game) {};
  
  // Gets Ghost position
  Position get_ghost_position();
  
  int make_ghost_move(int result);
  
};

#endif // SPOOKY_CHESS_GAME_H
