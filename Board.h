#pragma once

#include "Pieces.h"

using namespace std;

// information/operations on a square
class Square {
	public:
		pair<int, int> coord;

		vector<Piece*> pressure = {};

		Piece *piece = new Piece();

		void addPiece(Piece* this_piece);
		void removePiece();

		Piece getPiece();

		Square(pair<int, int> coord_);
		Square();
};

// organizes board information
class Board {
	public:
		// squares + pieces map
		vector<vector<Square>> board;

		// text board
		vector<vector<pair<char, char>>> strBoard;

		// pieces
		vector<Piece> whitePieces;
		vector<Piece> blackPieces;
		vector<Piece> pieces;

		int white_king_index = 0;
		int black_king_index = 0;


		// reset list of what pieces hit which squares
		void resetPressure();

		// delete piece forever
		void deletePiece(int index);


		// initializer -> configure board from text
		Board(vector<vector<pair<char, char>>> board_config);

		// can create empty object
		Board();
};