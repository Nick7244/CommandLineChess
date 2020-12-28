CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -g

play: Play.o Game.o ChessGame.o KingOfHill.o SpookyChessGame.o
	$(CXX) Play.o Game.o ChessGame.o KingOfHill.o SpookyChessGame.o -o play

Play.o: Play.cpp Game.h ChessGame.h KingOfHill.h SpookyChessGame.h Prompts.h
	$(CXX) $(CXXFLAGS) -c Play.cpp

Game.o: Game.cpp Game.h Piece.h Prompts.h Enumerations.h Terminal.h
	$(CXX) $(CXXFLAGS) -c Game.cpp

ChessGame.o: ChessGame.cpp Game.h ChessGame.h Piece.h ChessPiece.h Prompts.h Enumerations.h
	$(CXX) $(CXXFLAGS) -c ChessGame.cpp

KingOfHill.o: KingOfHill.cpp ChessGame.h
	$(CXX) $(CXXFLAGS) -c KingOfHill.cpp

SpookyChessGame.o: SpookyChessGame.cpp ChessGame.h
	$(CXX) $(CXXFLAGS) -c SpookyChessGame.cpp

clean:
	rm *.o play

