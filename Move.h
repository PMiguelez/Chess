#pragma once

#include "Board.h"

using namespace std;

// structure of a move
class Move {
	public:
	// piece to move
	Piece* piece;

	// square to go
	Square* square;

	pair<int, int> from;
	pair<int, int> to;


	// special cases
	Piece* secondPiece;
	Square* secondSquare;

	Piece promotedPiece;

	bool isCastle = false;
	bool isEnPassant = false;
	bool isPromotion = false;

	Move(Piece* this_piece, Square* this_square);
};