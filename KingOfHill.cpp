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

#include "KingOfHill.h"
#include "Game.h"
#include "Enumerations.h"

using std::ifstream;
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::ofstream;

// Creates new game in king of the hill chess start-of-game state 
KingOfHill::KingOfHill() : ChessGame() {}
 
// Creates game in king of the hill chess with state indicated in specified file
KingOfHill::KingOfHill(const std::string filename, int game_choice, bool& run_game): ChessGame(filename, game_choice, run_game) {}

int KingOfHill::check_win_cond(Position end, Position start, int result) {
  if((end.x == 3 || end.x == 4) && (end.y == 3 || end.y == 4)) {
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
    return MOVE_KING_OF_THE_HILL;
  }
  return 0;
}
