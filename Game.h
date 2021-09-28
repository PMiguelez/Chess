#pragma once

#include "MoveManager.h"

using namespace std;

// game flow controller -> connects most classes
class Game {
	public:
		Board board;
		Moves moves;

		Move lastMove = Move(new Piece(), new Square());

		char turn;
		int turnCount;

		void print();

		void deletePiece(int index);

		void changeTurn();

		void make_move(Move move);

		bool isLegal(Game newGame);


		// game loop | update active position
		vector<Game*> activate();


		// help organize game transition
		void cloneGameState(Game* game);

		// game to game transition
		Game(Game* parentGame, Move move);

		// initial constructor
		Game(vector<vector<pair<char, char>>> board_config);
};