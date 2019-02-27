using namespace std;

class Solution {
	public:
		int lenght;
		int total_benefit;
		int total_cost;

		vector<int> route;
		vector<int> milks_trucks;
		vector<int> satisfied_cuotes;
		vector<int> quality_by_route;
		vector<int> pivots;
		vector<bool> onetype;

		Solution(vector<int> vector);
		int random_index();
		int intra_evaluate(int old_eval, int index1, int index2, vector<vector<int>> cost_matrix);
		vector<int> neighbour_2opt_index(int index);
		vector<int> neighbour_move_index(int index);
};