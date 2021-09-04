#pragma once

#include "MoveManager.h"

using namespace std;

// returns a single array with all moves of a side
vector<Move> Moves::get_moves(char color) {
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
void Moves::updatePiece(int index, vector<vector<Square>>* board, Move lastMove) {

	// finding moves | updating main array
	moves[index] = getMoves(&(*pieces)[index], board, castle_white, castle_black, lastMove);

	// updating color array
	int colorIndex = (*pieces)[index].colorIndex;

	if ((*pieces)[index].color == 'W') {
		whiteMoves[colorIndex] = moves[index];
	}
	else {
		blackMoves[colorIndex] = moves[index];
	}

}

void Moves::deleteMoves(int index) {
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
Moves::Moves(vector<Piece>* this_pieces) {
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
Moves::Moves() {

}