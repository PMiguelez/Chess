#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>

#include "Calculation.h"

using namespace std;

// Game > Moves > Board > Pieces

int main()
{
	// reset random seed
	srand(time(0));

	// initial board
	vector<vector<pair<char, char>>> test = {
									{{'R','B'}, {'N','B'}, {'B','B'}, {'Q','B'}, {'K','B'}, {'B','B'}, {'N','B'}, {'R','B'}},
									{{'P','B'}, {'P','B'}, {'P','B'}, {'P','B'}, {'P','B'}, {'P','B'}, {'P','B'}, {'P','B'}},
									{{' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}},
									{{' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}},
									{{' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}},
									{{' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}},
									{{'P','W'}, {'P','W'}, {'P','W'}, {'P','W'}, {'P','W'}, {'P','W'}, {'P','W'}, {'P','W'}},
									{{'R','W'}, {'N','W'}, {'B','W'}, {'Q','W'}, {'K','W'}, {'B','W'}, {'N','W'}, {'R','W'}} };

	vector<vector<pair<char, char>>> test2 = {
								{{' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {'K','B'}, {' ',' '}, {' ',' '}, {' ',' '}},
								{{' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}},
								{{' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}},
								{{' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}},
								{{' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}},
								{{' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}},
								{{' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}},
								{{' ',' '}, {' ',' '}, {' ',' '}, {' ',' '}, {'K','W'}, {' ',' '}, {' ',' '}, {' ',' '}} };


	vector<Game*> games = Game(test).activate();
	vector<pair<float, int>> evals = {};

	while (true) {
		// evaluate positions
		evals.clear();
		for (int i = 0; i < games.size(); i++) {
			evals.push_back({calculate(games[i], 1), i});
		}
		sort(evals.begin(), evals.end());

		
		int game = 0;
		if ((*games[0]).turn == 'B') {
			game = games.size() - 1;
		}

		// clean memory
		games.erase(games.begin(), games.begin() + evals[game].second);
		games.erase(games.begin() + 1, games.end());

		// print game to console
		cin.ignore();
		(*games[0]).print();

		// make move
		games = (*games[0]).activate();
	}
}