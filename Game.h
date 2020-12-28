/* Derek Chang dchang38 dchang38@jhu.edu
 * Nick Maritato nmarita1 nmarita1@jhu.edu
 * Yunfei Li yli208 yli208@jhu.edu
 * 601.220
 * Final Project
 * 12/08/2017
 */

#ifndef GAME_H
#define GAME_H

#include <vector>
#include <map>
#include <string>
#include "Enumerations.h"
#include "Piece.h"
#include "Terminal.h"

// Game status code enumeration. Note that any value > 0
// indicates success, and any value < 0 indicates failure.
// You might want to use these as return codes from methods
// such as make_move().
enum status {
  LOAD_FAILURE = -10,
  SAVE_FAILURE,
  PARSE_ERROR,
  MOVE_ERROR_OUT_OF_BOUNDS,
  MOVE_ERROR_NO_PIECE,
  MOVE_ERROR_BLOCKED,
  MOVE_ERROR_CANT_CASTLE,
  MOVE_ERROR_MUST_HANDLE_CHECK,
  MOVE_ERROR_CANT_EXPOSE_CHECK,
  MOVE_ERROR_ILLEGAL,
  SUCCESS = 1,
  MOVE_CHECK,
  MOVE_CAPTURE,
  MOVE_CHECKMATE,
  MOVE_STALEMATE,
  MOVE_CASTLE,
  GAME_WIN,
  GAME_OVER,
  MOVE_KING_OF_THE_HILL,
  MOVE_CHECK_WHITE,
  MOVE_CHECK_BLACK,
  MOVE_CHECKMATE_WHITE,
  MOVE_CHECKMATE_BLACK,
  GHOSTED
};


// A base class representing a game that takes place on a chess board
class Game {

    // The type of a piece factory map. Maps from int describing a
    // Piece to the factory class producing the Piece.
    typedef std::map<int, AbstractPieceFactory*> PieceGenMap;

public:
    // Construct a board with the specified dimensions
    Game(unsigned int w = 8, unsigned int h = 8, int t = 1) :
        _width(w), _height(h), _pieces(w * h, nullptr), _turn(t) {}

    // Virtual destructor is necessary for a class with virtual methods
    virtual ~Game();

    // Return the width of the board
    unsigned int width() const { return _width; }

    // Return the height of the board
    unsigned int height() const { return _height; }

    // Create a piece on the board using the factory.
    // Returns true if the piece was successfully placed on the board
    bool init_piece(int piece_type, Player owner, Position pos);

    // Create a ghost piece on the board using the factory.
    // Returns true if the piece was successfully placed on the board
    //bool init_ghost_piece(int piece_type, Position pos);
    
    
    // Return a pointer to the piece at the specified position,
    // if the position is valid and occupied, nullptr otherwise.
    Piece* get_piece(Position pos) const;

    // Return the player whose turn it is
    Player player_turn() const { 
      return static_cast<Player>(!(_turn % 2)); 
    }

    // Return the current turn number (turn sequence number)
    int turn() const {
      return _turn;
    }

    // Return the current number of random number generations
    int num_rand_gen() const {
      return _num_rand_gen;
    }
    
    // Return true if the position is within bounds
    bool valid_position(Position pos) const {
        return pos.x < _width && pos.y < _height;
    }

    // gets symbol for piece
    std::string get_piece_type(int piece_type, Player p);
    
    // Displays board to cout
    void display_board();
    // prints result
    void get_result(int result);

    // Pure virtual function (i.e. not defined in Game)
    // so always need to override this in a subclass that
    // you want to instantiate.
    // Perform a move from the start Position to the end Position
    // The method returns an integer status where a value
    // >= 0 indicates SUCCESS, and a < 0 indicates failure
    virtual int make_move(Position start, Position end, int game_type) = 0;

    virtual int make_ghost_move(int result) = 0;
    
    virtual int save(const std::string filename, int game_choice) = 0;

    // Execute the main gameplay loop.
    virtual void run(int game_choice);

    // Pure virtual function (i.e. not defined in Game)
    // so always need to override this in subclasses
    // Reports whether the game is over.
    virtual bool game_over() const = 0;

protected:

    // Board dimensions
    unsigned int _width , _height;

    // Vector containing all the Pieces currently on the board
    std::vector<Piece*> _pieces;

    // Current game turn sequence number
    int _turn;

    // Current game number of random number generations
    int _num_rand_gen;
    
    // All the factories registered with this Board
    PieceGenMap _registered_factories;

    // Determine the 1D location index corresponding to a 2D position
    unsigned int index(Position pos) const {
        return pos.y * _width + pos.x;
    }

    // Functionality for creating a new piece (called by init_piece)
    Piece* new_piece(int piece_type, Player owner);

    // Functionality for adding piece factories (called by constructor)
    bool add_factory(AbstractPieceFactory* f);

};


#endif // GAME_H
