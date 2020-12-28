/* Derek Chang dchang38 dchang38@jhu.edu
 * Nick Maritato nmarita1 nmarita1@jhu.edu
 * Yunfei Li yli208 yli208@jhu.edu
 * 601.220
 * Final Project
 * 12/08/2017
 */

#ifndef KING_OF_HILL_H
#define KING_OF_HILL_H

#include "ChessGame.h"

class KingOfHill : public ChessGame {
  
 public:
  
  KingOfHill();
  
  KingOfHill(std::string filename, int game_choice, bool& run_game);
  
  int check_win_cond(Position end, Position start, int result) override;
  
};

#endif
