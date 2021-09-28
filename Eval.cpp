#include "Eval.h"

float eval(Game* game) {
	vector<vector<Square>> board = (*game).board.board;

	float space = 0;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			for (int i = 0; i < board[y][x].pressure.size(); i++) {
				if ((*board[y][x].pressure[i]).color == 'W') {
					space += 1;
				}
				else {
					space -= 1;
				}
			}
		}
	}
	
	return (*game).board.material_count + space / 16;
}