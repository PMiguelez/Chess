#pragma once

#include "Board.h"

using namespace std;

void Square::addPiece(Piece* this_piece) {
	piece = this_piece;
}
void Square::removePiece() {
	piece = new Piece();
}

Piece Square::getPiece() {
	return *piece;
}

Square::Square(pair<int, int> coord_) {
	coord = coord_;
}

Square::Square() {}


void Board::resetPressure() {
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			board[y][x].pressure = {};
		}
	}
}

// delete piece forever
void Board::deletePiece(int index) {
	if (pieces[index].color == 'W') {
		material_count -= pieces[index].value;
	}
	else {
		material_count += pieces[index].value;
	}
	pieces[index] = Piece();
}

// initializer -> configure board from text
Board::Board(vector<vector<pair<char, char>>> board_config) {
	strBoard = board_config;

	// for each coordinate on a board
	for (int y = 0; y < 8; y++) {
		board.push_back({});

		for (int x = 0; x < 8; x++) {
			char color = board_config[y][x].second;

			Piece piece;

			// check if there is a piece | creates it
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

			// add piece to proper arrays | update index (facilitate location)
			if (!piece.empty) {
				if (color == 'W') {
					piece.colorIndex = whitePieces.size();
					whitePieces.push_back(piece);

					if (piece.str == 'K') {
						white_king_index = pieces.size();
					}
					else {
						material_count += piece.value;
					}
				}
				else if (color == 'B') {
					piece.colorIndex = blackPieces.size();
					blackPieces.push_back(piece);

					if (piece.str == 'K') {
						black_king_index = pieces.size();
					}
					else {
						material_count -= piece.value;
					}
				}

				piece.index = pieces.size();
				pieces.push_back(piece);
			}

			// add a square on the board
			board[y].push_back(Square({ y, x }));
		}
	}
	// add pieces to their squares on the board map
	for (int i = 0; i < pieces.size(); i++) {
		board[pieces[i].pos.first][pieces[i].pos.second].addPiece(&pieces[i]);
	}

}

// can create empty object
Board::Board() {}