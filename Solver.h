using namespace std;

class Solver {
	public:
		int trucks_lenght;
		int milks_lenght;
		int farms_lenght;
		int global_quality;
		string name_instance;
		vector<int> plant_cuotes;
		vector<int> truck_capacities;
		vector<float> milk_values;
		vector<vector<int>> farms_locates;
		vector<vector<float>> cost_matrix;

		vector<float> result_times;
		vector<float> result_qualities;
		vector<int> global_solution;


		Solver(vector<int> capacities, vector<float> values, vector<vector<int>> locates, vector<int> cuotes, string instance);
		float evaluate(vector<int> solution);
		float fast_evaluate(vector<int> solution, float before_eval, int index);


		//Búsqueda Local
		Solution hill_climbing_new(int end_time);
		vector<int> hill_climbing(int end_time);
		vector<int> short_swap(vector<int> solution, int identity);
		vector<int> long_swap(vector<int> solution, int index, int move);
		vector<int> two_opt(vector<int> solution, int index1, int index2);
		vector<int> neighbour_index(vector<int> solution, int index);
		vector<int> random_feasible_solution();
		vector<int> random_solution();
		vector<int> random_int_vector(int lenght);

		//Utilities
		void print_int_vector(vector<int> array);
		void print_float_vector(vector<float> array);
		void print_farms_locates();
		void print_cost_matrix();
		string int_vector_to_string(vector<int> array);

		//Exports
		void export_result(vector<int> solution);
		void save_row_result();
		void draw_graph(vector<int> solution, int quality);
};