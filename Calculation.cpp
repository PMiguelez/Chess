#include "Calculation.h"

float calculate(Game* game, int depth) {
	// get positions
	vector<Game*> games = (*game).activate();

	// out of moves? -> end of the game
	if (games.empty()) {
		games.clear();
		(*game).changeTurn();

		// king (not) in check? -> Stalemate | Checkmate
		if ((*game).isLegal(*game)) {
			return 0;
		}
		else {
			if ((*game).turn == 'W') {
				return INFINITY;
			}
			return -INFINITY;
		}
	}

	// evaluate positions
	vector<pair<float, int>> evals = {};
	for (int i = 0; i < games.size(); i++) {
		evals.push_back({ eval(games[i]), i });
	}
	sort(evals.begin(), evals.end());


	int next_game = 0;
	if ((*games[0]).turn == 'B') {
		next_game = games.size() - 1;
	}

	// end of calculation -> return best evaluation
	if (depth == 0) {
		games.clear();
		return evals[next_game].first;
	}

	// evaluate deeper positions
	vector<pair<float, int>> new_evals = {};
	if ((*games[0]).turn == 'B') {
		for (int i = 0; i < games.size(); i++) {
			new_evals.push_back({ calculate(games[evals[i].second], depth - 1),  evals[i].second });
		}
	}
	else {
		for (int i = games.size() - 1; i >= 0; i--) {
			new_evals.push_back({ calculate(games[evals[i].second], depth - 1),  evals[i].second });
		}
	}

	sort(new_evals.begin(), new_evals.end());

	games.clear();

	// return evaluation
	return new_evals[next_game].first;
}