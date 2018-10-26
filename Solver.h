using namespace std;

class Solver {
	public:
		int trucks_lenght;
		int milks_lenght;
		int farms_lenght;
		vector<int> plant_cuotes;
		vector<int> truck_capacities;
		vector<float> milk_values;
		vector<vector<int>> farms_locates;

		vector<float> result_times;
		vector<float> result_qualities;


		void init(vector<int> capacities, vector<float> values, vector<vector<int>> locates, vector<int> cuotes);
		float evaluate(vector<int> solution);
		float fast_evaluate(vector<int> solution, float before_eval, int index);


		//Búsqueda Local
		vector<int> hill_climbing(int restarts);
		vector<int> neighbour(vector<int> solution, int identity);
		vector<int> random_feasible_solution();
		vector<int> random_solution();


		//Utilities
		void print_int_vector(vector<int> array);
		void print_float_vector(vector<float> array);
		void print_farms_locates();
		void export_result(vector<int> solution, string filename);
		void save_row_result(string filename);
};