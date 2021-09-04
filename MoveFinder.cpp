#pragma once

#include "MoveFinder.h"

using namespace std;

// find all moves of a piece on a board
bool inBound(int y, int x) {
	return (y >= 0 && y <= 7 && x >= 0 && x <= 7);
}

// special case: pawn promotion
void find_promotions(Move move, vector<Move>* moves) {
	move.isPromotion = true;

	move.promotedPiece = Knight(move.from, (*move.piece).color);
	(*moves).push_back(move);

	move.promotedPiece = Bishop(move.from, (*move.piece).color);
	(*moves).push_back(move);

	move.promotedPiece = Rook(move.from, (*move.piece).color);
	(*moves).push_back(move);

	move.promotedPiece = Queen(move.from, (*move.piece).color);
	(*moves).push_back(move);
}


vector<Move> pawn(Piece* piece, vector<vector<Square>>* board, Move lastMove) {
	vector<Move> moves = {};

	int y = (*piece).pos.first;
	int x = (*piece).pos.second;

	int thisY;

	// color affects squares to be checked
	if ((*piece).color == 'B') {
		thisY = y + 1;

		if (y == 1 && (*board)[y + 1][x].getPiece().empty && (*board)[y + 2][x].getPiece().empty) {
			moves.push_back(Move(piece, &(*board)[y + 2][x]));
		}
	}
	else {
		thisY = y - 1;

		if (y == 6 && (*board)[y - 1][x].getPiece().empty && (*board)[y - 2][x].getPiece().empty) {
			moves.push_back(Move(piece, &(*board)[y - 2][x]));
		}
	}

	// square in range? + square free from pieces?
		// is move a promotion?
		// adds move
		// marks pressure (if needed)

	if (inBound(thisY, x) && (*board)[thisY][x].getPiece().empty) {
		if (thisY == 7 || thisY == 0) {
			find_promotions(Move(piece, &(*board)[thisY][x]), &moves);
		}
		else {
			moves.push_back(Move(piece, &(*board)[thisY][x]));
		}
	}

	if (inBound(thisY, x - 1)) {
		(*board)[thisY][x - 1].pressure.push_back(piece);

		if (inBound(thisY, x - 1) && !(*board)[thisY][x - 1].getPiece().empty && (*piece).color != (*board)[thisY][x - 1].getPiece().color) {
			if (thisY == 7 || thisY == 0) {
				find_promotions(Move(piece, &(*board)[thisY][x - 1]), &moves);
			}
			else {
				moves.push_back(Move(piece, &(*board)[thisY][x - 1]));
			}
		}
	}

	if (inBound(thisY, x + 1)) {
		(*board)[thisY][x + 1].pressure.push_back(piece);

		if (!(*board)[thisY][x + 1].getPiece().empty && (*piece).color != (*board)[thisY][x + 1].getPiece().color) {
			if (thisY == 7 || thisY == 0) {
				find_promotions(Move(piece, &(*board)[thisY][x + 1]), &moves);
			}
			else {
				moves.push_back(Move(piece, &(*board)[thisY][x + 1]));
			}
		}
	}

	// special case: en passant
	if ((*lastMove.square).getPiece().str == 'P' && abs(lastMove.from.first - lastMove.to.first) == 2) {
		if (lastMove.to == pair<int, int>(y, x - 1)) {
			Move thisMove(piece, &(*board)[thisY][x - 1]);
			thisMove.secondSquare = &(*board)[y][x - 1];
			thisMove.isEnPassant = true;

			moves.push_back(thisMove);
		}
		else if (lastMove.to == pair<int, int>(y, x + 1)) {
			Move thisMove(piece, &(*board)[thisY][x + 1]);
			thisMove.secondSquare = &(*board)[y][x + 1];
			thisMove.isEnPassant = true;

			moves.push_back(thisMove);
		}
	}

	return moves;
}

vector<Move> knight(Piece* piece, vector<vector<Square>>* board) {
	vector<Move> moves = {};

	int y = (*piece).pos.first;
	int x = (*piece).pos.second;

	int thisY;
	int thisX;

	// goes through every reachable square
	for (int k = 1; k < 3; k++) {
		for (int right = -1; right < 2; right += 2) {
			for (int down = -1; down < 2; down += 2) {

				thisY = y + k * down;
				thisX = x + (3 - k) * right;

				// out of range -> break
				if (!inBound(thisY, thisX)) {
					continue;
				}

				// marks pressure
				(*board)[thisY][thisX].pressure.push_back(piece);

				// free from same color pieces?
				if ((*piece).color != (*board)[thisY][thisX].getPiece().color) {
					// adds move
					moves.push_back(Move(piece, &(*board)[thisY][thisX]));
				}
			}
		}
	}

	return moves;
}

vector<Move> bishop(Piece* piece, vector<vector<Square>>* board) {
	vector<Move> moves = {};

	int y = (*piece).pos.first;
	int x = (*piece).pos.second;

	int thisY;
	int thisX;

	// searches in all possible directions
	for (int direction1 = -1; direction1 < 2; direction1 += 2) {
		for (int direction2 = -1; direction2 < 2; direction2 += 2) {
			int k = 0;
			bool canMove = true;

			// goes through each square in a direction
			while (true) {
				k++;
				thisY = y + k * direction1;
				thisX = x + k * direction2;

				// out of range -> breaks
				if (!inBound(thisY, thisX)) {
					break;
				}

				Piece hitPiece = (*board)[thisY][thisX].getPiece();

				// marks pressure
				(*board)[thisY][thisX].pressure.push_back(piece);

				// free from same color pieces?
				if (hitPiece.color != (*piece).color) {

					// adds move
					if (canMove) {
						moves.push_back(Move(piece, &(*board)[thisY][thisX]));
					}

					// blocked by different color piece -> breaks
					if (!(*board)[thisY][thisX].getPiece().empty) {
						break;
					}
				}

				// blocked by same color piece
				else {
					canMove = false;

					// moves in same direction = not blocking pressure -> no break
					if (hitPiece.str != 'Q' && hitPiece.str != 'B') {

						// special pressure case: pawn blocking bishop
						if (hitPiece.str == 'P' && max(0, direction1) == ((*piece).color == 'B') && inBound(thisY + k * direction1, thisX + k * direction2)) {
							(*board)[thisY + k * direction1][thisX + k * direction2].pressure.push_back(piece);
						}

						break;
					}
				}
			}
		}
	}

	return moves;
}

vector<Move> rook(Piece* piece, vector<vector<Square>>* board) {
	vector<Move> moves = {};

	int y = (*piece).pos.first;
	int x = (*piece).pos.second;

	int thisY;
	int thisX;

	// searches in all possible directions
	for (int axis = 0; axis < 2; axis += 1) {
		thisY = y;
		thisX = x;
		for (int direction = -1; direction < 2; direction += 2) {
			int k = 0;
			bool canMove = true;

			// goes through each square in a direction
			while (true) {
				k++;
				if (axis == 0) {
					thisY = y + k * direction;
				}
				else {
					thisX = x + k * direction;
				}

				// out of range -> breaks
				if (!inBound(thisY, thisX)) {
					break;
				}

				Piece hitPiece = (*board)[thisY][thisX].getPiece();

				// marks pressure
				(*board)[thisY][thisX].pressure.push_back(piece);

				// free from same color pieces?
				if (hitPiece.color != (*piece).color) {

					// adds move
					if (canMove) {
						moves.push_back(Move(piece, &(*board)[thisY][thisX]));
					}

					// blocked by different color piece -> breaks
					if (!(*board)[thisY][thisX].getPiece().empty) {
						break;
					}
				}

				// blocked by same color piece
				else {
					canMove = false;

					// moves in same direction = not blocking pressure -> no break
					if (hitPiece.str != 'Q' && hitPiece.str != 'R') {
						break;
					}
				}
			}
		}
	}

	return moves;
}

vector<Move> queen(Piece* piece, vector<vector<Square>>* board) {
	// mix of bishop and rook movements

	vector<Move> moves = rook(piece, board);
	vector<Move> moves2 = bishop(piece, board);

	moves.insert(moves.end(), moves2.begin(), moves2.end());

	return moves;
}

vector<Move> king(Piece* piece, vector<vector<Square>>* board, pair<bool, bool> castle_white, pair<bool, bool> castle_black) {
	vector<Move> moves = {};

	int y = (*piece).pos.first;
	int x = (*piece).pos.second;

	int thisY;
	int thisX;

	// goes through all nearby squares
	for (int y_move = -1; y_move < 2; y_move++) {
		for (int x_move = -1; x_move < 2; x_move++) {
			thisY = y + y_move;
			thisX = x + x_move;

			// out of range -> breaks
			if (!inBound(thisY, thisX) || (y_move == 0 && x_move == 0)) {
				continue;
			}

			// marks pressure
			(*board)[thisY][thisX].pressure.push_back(piece);

			// square free from same color pieces? 
			if ((*piece).color != (*board)[thisY][thisX].getPiece().color) {
				// adds a move to the list
				moves.push_back(Move(piece, &(*board)[thisY][thisX]));
			}
		}
	}

	pair<bool, bool> castle = castle_white;
	if ((*piece).color == 'B') {
		castle = castle_black;
	}

	// castling left
	if (castle.first && (*board)[y][0].getPiece().hasMoved == false) {
		bool canCastle = true;
		for (int thisX = 1; thisX < x; thisX++) {
			if (!(*board)[y][thisX].getPiece().empty) {
				canCastle = false;
			}
		}

		if (canCastle) {
			Move thisMove(piece, &(*board)[y][x - 2]);
			thisMove.secondPiece = (*board)[y][0].piece;
			thisMove.secondSquare = &(*board)[y][x - 1];
			thisMove.isCastle = true;

			moves.push_back(thisMove);
		}
	}

	// castling right
	if (castle.second && (*board)[y][7].getPiece().hasMoved == false) {
		bool canCastle = true;
		for (int thisX = 6; thisX > x; thisX--) {
			if (!(*board)[y][thisX].getPiece().empty) {
				canCastle = false;
			}
		}

		if (canCastle) {
			Move thisMove(piece, &(*board)[y][x + 2]);
			thisMove.secondPiece = (*board)[y][7].piece;
			thisMove.secondSquare = &(*board)[y][x + 1];
			thisMove.isCastle = true;

			moves.push_back(thisMove);
		}
	}

	return moves;
}

// initializer -> determine which function to be called and calls it
vector<Move> getMoves(Piece* piece, vector<vector<Square>>* board, pair<bool, bool> castle_white, pair<bool, bool> castle_black, Move lastMove) {
	vector<Move> finalMoves;

	switch ((*piece).str) {
		case 'P':
			finalMoves = pawn(piece, board, lastMove);
			break;
		case 'N':
			finalMoves = knight(piece, board);
			break;
		case 'B':
			finalMoves = bishop(piece, board);
			break;
		case 'R':
			finalMoves = rook(piece, board);
			break;
		case 'Q':
			finalMoves = queen(piece, board);
			break;
		case 'K':
			finalMoves = king(piece, board, castle_white, castle_black);
			break;
	}

	return finalMoves;
}