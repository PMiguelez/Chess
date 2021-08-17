#pragma once

#include "Moves.h"

// game flow controller --> connects most classes
class Game {
	Board board;
	Moves moves;

	public:
		void make_move (Move move){
			// removing piece from its past position
			pair<int, int> from_coord = (*move.piece).pos;
			board.board[from_coord.first][from_coord.second].removePiece();

			// adding piece to new position
			(*move.square).addPiece(move.piece);
			(*move.piece).pos = (*move.square).coord;

			// updating string of the board
			pair<int, int> to_coord = (*move.square).coord;
			board.strBoard[to_coord.first][to_coord.second] = { (*move.piece).str, (*move.piece).color };
			board.strBoard[from_coord.first][from_coord.second] = { ' ', ' ' };

		}


		Game (vector<vector<pair<char, char>>> board_config) {

			// create the board | get ready to find moves
			board = Board(board_config);
			moves = Moves(&board.pieces);

			// game loop
			while (true) {

				// finds all moves for each piece
				for (int i = 0; i < board.pieces.size(); i++) {
					moves.updatePiece(i, &board.board);
				}

				// select one random move - make it
				vector<Move> allMoves = moves.get_moves();
				Move move = allMoves[int(rand() % (allMoves.size() - 1))];

				make_move(move);

				// prints board to console
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