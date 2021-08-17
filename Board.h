#pragma once

#include "Pieces.h"

using namespace std;

// organizes information / operations needed on a square
class Square {
	public:
		pair<int, int> coord;

		Piece *piece = new Piece();

		void addPiece(Piece *this_piece) {
			piece = this_piece;
		}
		void removePiece() {
			piece = new Piece();
		}
		Piece getPiece() {
			return *piece;
		}

		Square(pair<int, int> coord_) {
			coord = coord_;
		}

		Square() {

		};
};

// organizes board information
class Board {
	public:
		// all information on squares and pieces
		vector<vector<Square>> board;
		// text board
		vector<vector<pair<char, char>>> strBoard;
		// pieces
		vector<Piece> pieces;

		// initializer -> configuring board from texts
		Board(vector<vector<pair<char,char>>> board_config) {
			strBoard = board_config;

			// index of pieces (useful to locate them in arrays)
			int index = 0;

			// for each coordinate on a board
			for (int y = 0; y < 8; y++) {
				board.push_back({});

				for (int x = 0; x < 8; x++) {
					char color = board_config[y][x].second;

					// checks if there is a piece | creates and adds it to the array
					switch (board_config[y][x].first) {
						case ' ':
							break;

						case 'P':
							pieces.push_back(Pawn({ y,x }, color, index));
							break;

						case 'N':
							pieces.push_back(Knight({ y,x }, color, index));
							break;

						case 'B':
							pieces.push_back(Bishop({ y,x }, color, index));
							break;

						case 'R':
							pieces.push_back(Rook({ y,x }, color, index));
							break;

						case 'Q':
							pieces.push_back(Queen({ y,x }, color, index));
							break;

						case 'K':
							pieces.push_back(King({ y,x }, color, index));
							break;
					}
					
					// adds a square on the board
					board[y].push_back(Square({y, x}));

					if (pieces.size() > index) {
						index++;
					}
				}
			}
			// for each piece -> adds it to it's square on the board
			for (int i = 0; i < pieces.size(); i++) {
				board[pieces[i].pos.first][pieces[i].pos.second].addPiece(&pieces[i]);
			}

		}

		// hability to create empty object
		Board() {

		}
};