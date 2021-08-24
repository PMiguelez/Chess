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
		vector<Piece> whitePieces;
		vector<Piece> blackPieces;
		vector<Piece> pieces;

		// initializer -> configuring board from texts
		Board(vector<vector<pair<char,char>>> board_config) {
			strBoard = board_config;

			// for each coordinate on a board
			for (int y = 0; y < 8; y++) {
				board.push_back({});

				for (int x = 0; x < 8; x++) {
					char color = board_config[y][x].second;

					Piece piece;

					// checks if there is a piece | creates it
					switch (board_config[y][x].first) {
						case ' ':
							break;

						case 'P':
							piece = Pawn({ y,x }, color);
							break;

						case 'N':
							piece = Knight({ y,x }, color);
							break;

						case 'B':
							piece = Bishop({ y,x }, color);
							break;

						case 'R':
							piece = Rook({ y,x }, color);
							break;

						case 'Q':
							piece = Queen({ y,x }, color);
							break;

						case 'K':
							piece = King({ y,x }, color);
							break;
					}
					
					// adds piece to proper arrays | update index
					if (!piece.empty) {
						if (color == 'W') {
							piece.colorIndex = whitePieces.size();
							whitePieces.push_back(piece);
						}
						else if (color == 'B') {
							piece.colorIndex = blackPieces.size();
							blackPieces.push_back(piece);
						}

						piece.index = pieces.size(); // useful to locate them in arrays
						pieces.push_back(piece);
					}
					
					// adds a square on the board
					board[y].push_back(Square({y, x}));
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