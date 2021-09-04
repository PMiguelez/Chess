#pragma once

#include "MoveFinder.h"

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

	pair<bool, bool> castle_white = { true, true };
	pair<bool, bool> castle_black = { true, true };

	// returns a single array with all moves of a side
	vector<Move> get_moves(char color);

	// finds and stores a piece's moves (given a board)
	void updatePiece(int index, vector<vector<Square>>* board, Move lastMove);

	void deleteMoves(int index);

	// initializer -> get pieces
	Moves(vector<Piece>* this_pieces);

	// hability to create empty object
	Moves();
};