#pragma once

#include "Move.h"

using namespace std;

Move::Move(Piece* this_piece, Square* this_square) {
	piece = this_piece;
	square = this_square;

	from = (*this_piece).pos;
	to = (*this_square).coord;
}