#pragma once

#include "Game.h"

using namespace std;

void Game::print() {
	cout << "Turn #" << int(turnCount / 2) << "   Next move: " << turn << "\n";
	turnCount++;

	for (int y = 0; y < board.strBoard.size(); y++) {
		for (int x = 0; x < board.strBoard[y].size(); x++) {
			if (!(*board.board[y][x].piece).empty) {
				cout << board.strBoard[y][x].first << board.strBoard[y][x].second << " ";
			}
			else {
				cout << "-- ";
			}
		}
		cout << "\n";
	}

	cout << "\n\n\n";
}


void Game::deletePiece(int index) {
	moves.deleteMoves(index);
	board.deletePiece(index);
}


void Game::changeTurn() {
	turnCount++;

	if (turn == 'W') {
		turn = 'B';
	}
	else {
		turn = 'W';
	}
}


void Game::make_move(Move move) {
	// rook moves from starting position -> can no longer castle
	if ((*move.piece).str == 'R') {
		if ((*move.piece).pos.first == 0) {
			if ((*move.piece).pos.second == 0) {
				moves.castle_black.first = false;
			}
			else if ((*move.piece).pos.second == 7) {
				moves.castle_black.second = false;
			}
		}
		else if ((*move.piece).pos.first == 7) {
			if ((*move.piece).pos.second == 0) {
				moves.castle_white.first = false;
			}
			else if ((*move.piece).pos.second == 7) {
				moves.castle_white.second = false;
			}
		}
	}

	// castle? king move? -> can no longer castle
	if (move.isCastle || (*move.piece).str == 'K') {
		if ((*move.piece).color == 'W') {
			moves.castle_white = { false, false };
		}
		else {
			moves.castle_black = { false, false };
		}
	}

	if (move.isCastle) {
		// move rook
		make_move(Move(move.secondPiece, move.secondSquare));
	}

	if (move.isEnPassant) {
		// remove en-passaned piece
		deletePiece((*move.secondSquare).getPiece().index);
	}

	if (move.isPromotion) {
		// pawn = promoted piece
		Piece beforePromotion = (*move.piece);
		(*move.piece) = move.promotedPiece;

		// adjust it's properties
		(*move.piece).index = beforePromotion.index;
		(*move.piece).colorIndex = beforePromotion.colorIndex;
		(*move.piece).hasMoved = true;
	}

	// removing piece from its past position
	pair<int, int> from_coord = (*move.piece).pos;
	board.board[from_coord.first][from_coord.second].removePiece();

	// deleting captured piece
	if (!(*move.square).getPiece().empty) {
		deletePiece((*move.square).getPiece().index);
	}

	// adding piece to new position
	(*move.square).addPiece(move.piece);
	(*move.piece).pos = (*move.square).coord;

	// updating string of the board
	pair<int, int> to_coord = (*move.square).coord;
	board.strBoard[to_coord.first][to_coord.second] = { (*move.piece).str, (*move.piece).color };
	board.strBoard[from_coord.first][from_coord.second] = { ' ', ' ' };

	lastMove = move;
	changeTurn();
}


bool Game::isLegal(Game newGame) {
	// what king can't move this turn?
	int kingIndex = newGame.board.white_king_index;
	if (newGame.turn == 'W') {
		kingIndex = newGame.board.black_king_index;
	}

	pair<int, int> kingPos = newGame.board.pieces[kingIndex].pos;

	// checking attacks on it
	vector<Piece*> pressure = newGame.board.board[kingPos.first][kingPos.second].pressure;
	for (int i = 0; i < pressure.size(); i++) {
		if ((*pressure[i]).color == newGame.turn) {
			return false;
		}
	}

	return true;
}


// game loop | new active position
vector<Game*> Game::activate() {
	// get all moves
	vector<Move> allMoves = moves.get_moves(turn);

	vector<Game*> newGames = {};

	// making a game for each move
	for (int i = 0; i < allMoves.size(); i++) {
		newGames.push_back(new Game(this, allMoves[i]));

		// is illegal? -> removes it
		if (!isLegal(*newGames[newGames.size() - 1])) {
			newGames.pop_back();
		}
	}

	// out of moves? -> end
	if (newGames.empty()) {
		changeTurn();

		// king (not) in check? -> Stalemate | Checkmate
		if (isLegal(*this)) {
			cout << "\n\n DRAW BY STALEMATE ";
		}
		else {
			cout << "\n\n CHECKMATE " << turn << " WINS";
		}
		exit(1000);
	}

	return newGames;
}


// organize game transition
void Game::cloneGameState(Game* game) {

	// create the board | get ready to find moves
	board = Board((*game).board.strBoard);
	moves = Moves(&board.pieces);

	// can castle?
	moves.castle_white = (*game).moves.castle_white;
	moves.castle_black = (*game).moves.castle_black;

	// adjust turn
	turn = (*game).turn;
	turnCount = (*game).turnCount;
}


// game to game transition
Game::Game(Game* parentGame, Move move) {

	// clone vars from past Game
	cloneGameState(parentGame);

	// adjust values to this Game
	move.piece = board.board[move.from.first][move.from.second].piece;
	move.square = &board.board[move.to.first][move.to.second];
	if (move.isCastle) {
		move.secondPiece = board.board[(*move.secondPiece).pos.first][(*move.secondPiece).pos.second].piece;
		move.secondSquare = &board.board[(*move.secondSquare).coord.first][(*move.secondSquare).coord.second];
	}
	if (move.isEnPassant) {
		move.secondSquare = &board.board[(*move.secondSquare).coord.first][(*move.secondSquare).coord.second];
	}

	make_move(move);

	// update moves + pressure
	for (int i = 0; i < board.pieces.size(); i++) {
		if (!board.pieces[i].empty) {
			moves.updatePiece(i, &board.board, move);
		}
	}
}


// initial constructor
Game::Game(vector<vector<pair<char, char>>> board_config) {

	// create the board | get ready to find moves
	board = Board(board_config);
	moves = Moves(&board.pieces);

	turn = 'W';
	turnCount = 1;

	// finds all moves + pressure
	for (int i = 0; i < board.pieces.size(); i++) {
		if (!board.pieces[i].empty) {
			moves.updatePiece(i, &board.board, Move(new Piece(), new Square()));
		}
	}
}
