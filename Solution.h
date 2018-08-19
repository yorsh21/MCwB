using namespace std;

class Solution {
	public:
		int truck_lenght;
		int farm_lenght;
		vector<int> plant_cuotes;
		vector<int> truck_capacities;
		vector<float> milk_values;
		vector<vector<int>> farms_locates;

		void init(vector<int> capacities, vector<float> values, vector<vector<int>> locates, vector<int> cuotes);
		float evaluate(vector<int> solution, bool show);


		//Búsqueda Local
		void hill_climbing(int restarts);
		vector<int> neighbour(vector<int> solution, int identity);
		vector<int> random_solution();


		//Utilities
		void print_int_vector(vector<int> array);
		void print_float_vector(vector<float> array);
		void print_farms_locates();
};