using namespace std;

class Solver {
	public:
		string name_instance;
		int trucks_lenght;
		int milks_lenght;
		int farms_lenght;
		int global_quality;
		vector<int> global_trucks_position;
		vector<int> plant_cuotes;
		vector<int> truck_capacities;
		vector<float> milk_values;
		vector<vector<int>> farms_locates;
		vector<vector<int>> cost_matrix;

		vector<float> result_times;
		vector<float> result_qualities;
		vector<int> global_solution;
		
		vector<int> milks_trucks;
		vector<int> satisfied_cuotes;
		vector<int> quality_by_route;
		vector<int> pivots;
		vector<bool> onetype;
		int milk_income;


		Solver(vector<int> capacities, vector<float> values, vector<vector<int>> locates, vector<int> cuotes, string instance);
		int evaluate(vector<int> solution, bool show);
		int intra_evaluate(vector<int> solution, int old_eval, int index1, int index2);
		int extra_evaluate(vector<int> solution, int old_eval, int index1, int index2);
		int random_index(vector<int> solution);


		//Búsqueda Local
		vector<int> hill_climbing(int end_time);
		vector<int> short_swap(vector<int> solution, int identity);
		vector<int> long_swap(vector<int> solution, int index, int move);
		vector<int> move_extra_routes(vector<int> solution, int index1, int index2);
		vector<int> two_opt(vector<int> solution, int index1, int index2);
		vector<int> neighbour_2opt_index(vector<int> solution, int index);
		vector<int> neighbour_move_index(vector<int> solution, int index);
		vector<int> random_feasible_solution();
		vector<int> random_solution();
		vector<int> random_int_vector(int lenght);
		vector<int> random_assignment(vector<int> array);

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