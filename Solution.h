using namespace std;

class Solution {
	public:
		int nodes_lenght;
		int farms_lenght;
		int milks_lenght;
		int trucks_lenght;

		vector<int> index_farms;
		vector<int> lenght_routes;
		vector<int> list_milks;
		vector<float> cost_routes;
		vector<vector<int>> farms;

		vector<int> nodes;
		vector<int> qualities;

		Solution(vector<vector<int>> farms_locates, int milks, int trucks);
		int neighbour_lenght();
		float evaluate();
		float get_neighbour(int index);
		float set_neighbour(int index);

		vector<int> random_int_vector(int lenght);
};