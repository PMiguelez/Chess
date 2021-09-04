#pragma once

#include "Move.h"

using namespace std;

// is a position in the board?
bool inBound(int y, int x);

// special case: pawn promotion
void find_promotions(Move move, vector<Move>* moves);


vector<Move> pawn(Piece* piece, vector<vector<Square>>* board, Move lastMove);

vector<Move> knight(Piece* piece, vector<vector<Square>>* board);

vector<Move> bishop(Piece* piece, vector<vector<Square>>* board);

vector<Move> rook(Piece* piece, vector<vector<Square>>* board);

vector<Move> queen(Piece* piece, vector<vector<Square>>* board);

vector<Move> king(Piece* piece, vector<vector<Square>>* board, pair<bool, bool> castle_white, pair<bool, bool> castle_black);

// find all moves of a piece on a board
vector<Move> getMoves(Piece* piece, vector<vector<Square>>* board, pair<bool, bool> castle_white, pair<bool, bool> castle_black, Move lastMove);