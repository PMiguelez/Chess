#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// information/operations on pieces
class Piece {
	public:
		int value = 0;
		char color = ' ';
		char str = ' ';

		// secondary values
		bool empty = false;
		bool hasMoved = false;
		int index = -1;
		int colorIndex = -1;

		pair<int, int> pos = {};

		Piece(pair<int, int> getPos, char this_color) {
			color = this_color;
			pos = getPos;
		}

		Piece() {
			empty = true;
			hasMoved = true;
		}
};



class Pawn : public Piece {
	public:
		Pawn(pair<int, int> getPos, char this_color) : Piece(getPos, this_color) {
			value = 1;
			str = 'P';
		};
};


class Knight : public Piece {
	public:
		Knight(pair<int, int> getPos, char this_color) : Piece(getPos, this_color) {
			value = 3;
			str = 'N';
		};
};


class Bishop : public Piece {
	public:
		Bishop(pair<int, int> getPos, char this_color) : Piece(getPos, this_color) {
			value = 3;
			str = 'B';
		};
};


class Rook : public Piece {
	public:
		Rook(pair<int, int> getPos, char this_color) : Piece(getPos, this_color) {
			value = 5;
			str = 'R';
		};
};


class Queen : public Piece {
	public:
		Queen(pair<int, int> getPos, char this_color) : Piece(getPos, this_color) {
			value = 9;
			str = 'Q';
		};
};


class King : public Piece {
	public:
		King(pair<int, int> getPos, char this_color) : Piece(getPos, this_color) {
			value = 100000;
			str = 'K';
		}
};