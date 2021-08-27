#pragma once

#include "Moves.h"

// game flow controller --> connects most classes
class Game {
	Board board;
	Moves moves;

	public:
		void deletePiece(int index) {
			moves.deleteMoves(index);
			board.deletePiece(index);
		}

		void make_move(Move move) {
			(*move.piece).hasMoved = true;

			if (move.isCastle) {
				make_move(Move(move.secondPiece, move.secondSquare));
			}

			if (move.isEnPassant) {
				deletePiece((*move.secondSquare).getPiece().index);
			}

			if (move.isPromotion) {
				Piece beforePromotion = (*move.piece);
				(*move.piece) = move.promotedPiece;

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

	}


	Game(vector<vector<pair<char, char>>> board_config) {

		// create the board | get ready to find moves
		board = Board(board_config);
		moves = Moves(&board.pieces);

		Move lastMove(new Piece(), new Square());

		char turn = 'W';
		int turnCount = 1;

		// game loop
		while (true) {

			cin.ignore();

			// finds all moves for each piece
			for (int i = 0; i < board.pieces.size(); i++) {
				if (!board.pieces[i].empty) {
					moves.updatePiece(i, &board.board, lastMove);
				}
			}

			// find all moves
			vector<Move> allMoves = moves.get_moves(turn);

			// check if player is out of moves
			if (allMoves.empty()) {
				cout << "\n\n DRAW BY STALEMATE";
				break;
			}

			// select one random move - make it
			Move move = allMoves[int(rand() % (allMoves.size()))];

			make_move(move);

			lastMove = move;

			// changes turn
			if (turn == 'W') {
				turn = 'B';
			}
			else {
				turn = 'W';
			}

			// prints board to console
			cout << "Turn #" << turnCount << "\n";
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
	}
};