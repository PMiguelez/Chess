#pragma once

#include "Board.h"

// structure of a move
class Move {
	public:
		// points to a piece and a square
		Piece *piece;
		Square *square;

		pair<int, int> from;
		pair<int, int> to;

		// special cases
		Piece* secondPiece;
		Square* secondSquare;

		Piece promotedPiece;

		bool isCastle = false;
		bool isEnPassant = false;
		bool isPromotion = false;

		Move(Piece* this_piece, Square* this_square) {
			piece = this_piece;
			square = this_square;

			from = (*this_piece).pos;
			to = (*this_square).coord;
		}
};

// finds all moves of a piece on a board
class getMoves {
	public:
		vector<Move> finalMoves;

		// is a position in the board? -> useful
		bool inBound(int y, int x) {
			return (y >= 0 && y <= 7 && x >= 0 && x <= 7);
		}

		// special case: pawn promotion
		void find_promotions(Move move, vector<Move> *moves){
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

		// EACH PIECE'S ALGORITHM FOR FINDING ITS MOVES:

		vector<Move> pawn(Piece *piece, vector<vector<Square>> *board, Move lastMove) {
			vector<Move> moves = {};

			int y = (*piece).pos.first;
			int x = (*piece).pos.second;

			int thisY;

			// color affects squares to be checked
			if ((*piece).getColor() == 'B') {
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

			// does the square exist? + is it free from pieces?
				// is it a promotion?
				// adds a move to the list

			if (inBound(thisY, x) && (*board)[thisY][x].getPiece().empty) {
				if (thisY == 7 || thisY == 0) {
					find_promotions(Move(piece, &(*board)[thisY][x]), &moves);
				}
				else {
					moves.push_back(Move(piece, &(*board)[thisY][x]));
				}
			}
			if (inBound(thisY, x - 1) && ! (*board)[thisY][x - 1].getPiece().empty && (*piece).getColor() != (*board)[thisY][x - 1].getPiece().getColor()) {
				if (thisY == 7 || thisY == 0) {
					find_promotions(Move(piece, &(*board)[thisY][x - 1]), &moves);
				}
				else {
					moves.push_back(Move(piece, &(*board)[thisY][x - 1]));
				}
			}
			if (inBound(thisY, x + 1) && ! (*board)[thisY][x + 1].getPiece().empty && (*piece).getColor() != (*board)[thisY][x + 1].getPiece().getColor()) {
				if (thisY == 7 || thisY == 0) {
					find_promotions(Move(piece, &(*board)[thisY][x + 1]), &moves);
				}
				else {
					moves.push_back(Move(piece, &(*board)[thisY][x + 1]));
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

		vector<Move> knight(Piece *piece, vector<vector<Square>> *board) {
			vector<Move> moves = {};

			int y = (*piece).pos.first;
			int x = (*piece).pos.second;

			int thisY;
			int thisX;

			// goes through every reachable square
			for (int k = 1; k < 3; k++) {
				for (int right = -1; right < 2; right += 2) {
					for (int down = -1; down < 2; down += 2) {

						thisY = y + k * right;
						thisX = x + (3 - k) * down;

						// does it exist? + is it free?
						if (inBound(thisY, thisX) && (*piece).getColor() != (*board)[thisY][thisX].getPiece().getColor()) {
							// adds a move to the list
							moves.push_back(Move(piece, &(*board)[thisY][thisX]));
						}
					}
				}
			}

			return moves;
		}

		vector<Move> bishop(Piece *piece, vector<vector<Square>> *board) {
			vector<Move> moves = {};

			int y = (*piece).pos.first;
			int x = (*piece).pos.second;

			int thisY;
			int thisX;

			// searches in all possible directions
			for (int direction1 = -1; direction1 < 2; direction1 += 2) {
				for (int direction2 = -1; direction2 < 2; direction2 += 2) {
					int k = 0;

					// goes through each square in a direction
					while (true) {
						k++;
						thisY = y + k * direction1;
						thisX = x + k * direction2;

						// does it exist? + is it free from the same color? 
						if (inBound(thisY, thisX) && (*board)[thisY][thisX].getPiece().getColor() != (*piece).getColor()) {
							// adds a move to the list
							moves.push_back(Move(piece, &(*board)[thisY][thisX]));

							// is it blocked by a different color? -> changes direction
							if (!(*board)[thisY][thisX].getPiece().empty) {
								break;
							}
						}
						// changes direction
						else {
							break;
						}
					}
				}
			}

			return moves;
		}

		vector<Move> rook(Piece *piece, vector<vector<Square>> *board) {
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

					// goes through each square in a direction
					while (true) {
						k++;
						if (axis == 0) {
							thisY = y + k * direction;
						}
						else {
							thisX = x + k * direction;
						}

						// does it exist? + is it free from the same color? 
						if (inBound(thisY, thisX) && (*board)[thisY][thisX].getPiece().getColor() != (*piece).getColor()) {
							// adds a move to the list
							moves.push_back(Move(piece, &(*board)[thisY][thisX]));

							// is it blocked by a different color ? -> changes direction
							if (!(*board)[thisY][thisX].getPiece().empty) {
								break;
							}
						}

						// changes direction
						else {
							break;
						}
					}
				}
			}

			return moves;
		}

		vector<Move> queen(Piece *piece, vector<vector<Square>> *board) {
			// mix of bishop and rook movements

			vector<Move> moves = rook(piece, board);
			vector<Move> moves2 = bishop(piece, board);

			moves.insert(moves.end(), moves2.begin(), moves2.end());

			return moves;
		}

		vector<Move> king(Piece *piece, vector<vector<Square>> *board) {
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

					// does it exist? + is it free? 
					if (inBound(thisY, thisX) && (y_move != 0 || x_move != 0) && (*piece).getColor() != (*board)[thisY][thisX].getPiece().getColor()) {
						// adds a move to the list
						moves.push_back(Move(piece, &(*board)[thisY][thisX]));
					}
				}
			}

			// castling left
			if ((*piece).hasMoved == false && (*board)[y][0].getPiece().hasMoved == false){
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
			if ((*piece).hasMoved == false && (*board)[y][7].getPiece().hasMoved == false) {
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
		getMoves(Piece *piece, vector<vector<Square>> *board, Move lastMove) {
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
					finalMoves = king(piece, board);
					break;
			}
		}
};

// organizes all moves
class Moves {
	vector<Piece>* whitePieces;
	vector<Piece>* blackPieces;
	vector<Piece>* pieces;

	public:
	// moves are organized by pieces (in multiple arrays)
	vector<vector<Move>> whiteMoves;
	vector<vector<Move>> blackMoves;
	vector<vector<Move>> moves;

	// returns a single array with all moves of a side
	vector<Move> get_moves(char color) {
		vector<vector<Move>>* this_moves = &whiteMoves;

		if (color == 'B') {
			this_moves = &blackMoves;
		}

		vector<Move> allMoves;

		for (int i = 0; i < (*this_moves).size(); i++) {
			for (int k = 0; k < (*this_moves)[i].size(); k++) {
					allMoves.push_back((*this_moves)[i][k]);
			}
		}

		return allMoves;
	}

	// finds and stores a piece's moves (given a board)
	void updatePiece(int index, vector<vector<Square>>* board, Move lastMove) {

		// finding moves | updating main array
		moves[index] = getMoves(&(*pieces)[index], board, lastMove).finalMoves;

		// updating color array
		int colorIndex = (*pieces)[index].colorIndex;

		if ((*pieces)[index].color == 'W') {
			whiteMoves[colorIndex] = moves[index];
		}
		else {
			blackMoves[colorIndex] = moves[index];
		}

	}

	void deleteMoves(int index) {
		moves[index] = {};

		// updating color array
		int colorIndex = (*pieces)[index].colorIndex;

		if ((*pieces)[index].color == 'W') {
			whiteMoves[colorIndex] = {};
		}
		else {
			blackMoves[colorIndex] = {};
		}
	}

	// initializer -> get pieces
	Moves(vector<Piece>* this_pieces) {
		pieces = this_pieces;

		for (int i = 0; i < (*pieces).size(); i++) {
			moves.push_back({});

			if ((*pieces)[i].color == 'W') {
				whiteMoves.push_back({});
			}
			else {
				blackMoves.push_back({});
			}
		}
	}

	// hability to create empty object
	Moves() {

	}
};