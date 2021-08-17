#pragma once

using namespace std;

// information and operations with pieces
class Piece {
	protected:
		int value = 0;
	public:
		bool empty = false;
		int index = -1;
		char color = ' ';
		char str = ' ';

		pair<int, int> pos = {};

		char getColor() {
			return color;
		}
		int getValue() {
			return value;
		}

		Piece(pair<int, int> getPos, char this_color, int this_index) {
			index = this_index;
			color = this_color;
			pos = getPos;
		}

		Piece() {
			empty = true;
		}
};



class Pawn : public Piece {
	public:
		Pawn(pair<int, int> getPos, char this_color, int this_index) : Piece(getPos, this_color, this_index) {
			value = 1;
			str = 'P';
		};
};


class Knight : public Piece {
	public:
		Knight(pair<int, int> getPos, char this_color, int this_index) : Piece(getPos, this_color, this_index) {
			value = 3;
			str = 'N';
		};
};


class Bishop : public Piece {
	public:
		Bishop(pair<int, int> getPos, char this_color, int this_index) : Piece(getPos, this_color, this_index) {
			value = 3;
			str = 'B';
		};
};


class Rook : public Piece {
	public:
		Rook(pair<int, int> getPos, char this_color, int this_index) : Piece(getPos, this_color, this_index) {
			value = 5;
			str = 'R';
		};
};


class Queen : public Piece {
	public:
		Queen(pair<int, int> getPos, char this_color, int this_index) : Piece(getPos, this_color, this_index) {
			value = 9;
			str = 'Q';
		};
};


class King : public Piece {
	public:
		King(pair<int, int> getPos, char this_color, int this_index) : Piece(getPos, this_color, this_index) {
			value = 100000;
			str = 'K';
		};
};