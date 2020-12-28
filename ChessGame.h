/* Derek Chang dchang38 dchang38@jhu.edu
 * Nick Maritato nmarita1 nmarita1@jhu.edu
 * Yunfei Li yli208 yli208@jhu.edu
 * 601.220
 * Final Project
 * 12/08/2017 
 */

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "Game.h"
#include "ChessPiece.h"
#include <vector>

class ChessGame : public Game {

public:

    // Creates new game in standard start-of-game state 
    ChessGame();

    // Creates game with state indicated in specified file
    ChessGame(std::string filename, int game_choice, bool& run_game);

    // Check which column a piece is in
    int check_coln(char c);
    
    // Convert to column
    char convert_coln(int i);
    
    // Checks if a piece is blocked
    // Uses the trajectory vector and move type variable
    bool check_blocked(const Piece *p, const std::vector<Position> &trajectory, const int type_move, Player player);

    // Gets King position of a specific player
    Position get_king_position(Player player);
    
    // Gets King position of a specific player ignoring where king moved from
    Position get_king_position(Player player, Position exempt);

    // Edits trajectory vector to get rid of exempt positions
    bool edit_trajectory(const Piece* p, std::vector<Position>& trajectory, Position pos, Position exempt1, Position exempt2, int success);
    
    // Checks if king is in check
    bool check_check(Position king, Position exempt1, Position exempt2, Player player, Position end, int result, bool checking_for_self);

    // Checks if piece has no possible moves
    bool check_stale(Position pos, Position exempt, Player player);

    // Checks if a player is in stalemate
    bool check_stalemate(Position exempt, Player player);
    
    // Completes castle move, returns position of rook after move
    Position castle(Position end);

    // checks castle move is valid
    Position check_castle(Position end, Position king, Position temp);

    // checks if castle in a direction is invalid
    void castle_valid(Piece *piece, Position start);

    // check move is valid and piece is not a pawn attacking an empty space
    bool check_move_valid(Piece* target, Piece* piece, int type_move);

    // check for checks after move, checkmate, and stalemate
    int check_move_check(int type, bool cur, Piece* piece, Position king, Position start, Position end, Position temp, int type_move, int game_type);
    
    /* Function to check type of move and if move isn't blocked. */
    int check_move(bool cur, Piece* piece, Piece* target, int type_move, Position king, Position start, Position end, int game_type);
    
    // Perform a move from the start Position to the end Position
    // The method returns an integer with the status
    // >= 0 is SUCCESS, < 0 is failure
    int make_move(Position start, Position end, int game_type) override;

    // check variation win condition
    virtual int check_win_cond(Position end, Position start, int result) {return (result + end.x + start.x);}

    // Move ghost piece
    virtual int make_ghost_move(int result) override {return result;}
    
    // Performs the saving of the game state
    // returns 0 if success, returns 1 if failure
    int save(const std::string filename, int game_choice) override;
    
    // Reports whether the chess game is over
    bool game_over() const override;

protected:

    // Create all needed factories for the kinds of pieces
    // used in chess (doesn't make the actual pieces)
    virtual void initialize_factories();
    
};

#endif // CHESS_GAME_H
