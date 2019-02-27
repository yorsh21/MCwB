#include "Solution.h"


Solution::Solution(vector<int> vector) {
	route = vector;
	lenght = (int)vector.size();
	total_benefit = 0;
	total_cost = 0;

	milks_trucks = {};
	satisfied_cuotes = {};
	quality_by_route = {};
	pivots = {};
	onetype = {};
}


int Solution::random_index() {
	int index = 0;
	while(route[index] == 0) {
		index = rand() % (lenght - 2) + 1;
	}

	return index;
}

vector<int> Solution::neighbour_2opt_index(int index) {
	vector<int> indexes;

	int count = 1;
	while(index + count < lenght && route[index + count] != 0) {
		indexes.push_back(index + count);
		count++;
	}

	count = 1;
	while(index - count > 0 && route[index - count] != 0) {
		indexes.push_back(index - count);
		count++;
	}

	return indexes;
}

vector<int> Solution::neighbour_move_index(int index) {
	vector<int> indexes;

	int route1 = -1;
	int route2 = -1;
	for (int i = 0; i < lenght; ++i) {
		if (route[i] == 0) {
			if (route1 != -1 && route2 == -1 && i >= index){
				route2 = i;
			}

			if (i < index){
				route1 = i;
			}
		}
	}

	for (int i = 1; i < route1; ++i) {
		if (route[i] != 0) {
			indexes.push_back(i);
		}
	}

	for (int i = route2; i < lenght; ++i) {
		if (route[i] != 0) {
			indexes.push_back(i);
		}
	}

	return indexes;
}


int Solution::intra_evaluate(int old_eval, int index1, int index2, vector<vector<int>> cost_matrix) {
	int new_eval = old_eval;

	int k1 = min(index1, index2);
	int k2 = max(index1, index2);

	new_eval += cost_matrix[route[k1]][route[k1-1]];
	new_eval += cost_matrix[route[k2]][route[k2+1]];
	
	new_eval -= cost_matrix[route[k2]][route[k1-1]];
	new_eval -= cost_matrix[route[k1]][route[k2+1]];

	return new_eval;
}