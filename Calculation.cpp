#include "Calculation.h"

float calculate(Game* game, int depth) {
	vector<Game*> games = (*game).activate();

	vector<pair<float, int>> evals = {};
	for (int i = 0; i < games.size(); i++) {
		evals.push_back({ eval(games[i]), i });
	}
	sort(evals.begin(), evals.end());


	int next_game = 0;
	if ((*games[0]).turn == 'B') {
		next_game = games.size() - 1;
	}

	if (depth == 0) {
		return evals[next_game].first;
	}

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

	return new_evals[next_game].first;
}