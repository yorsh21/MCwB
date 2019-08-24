using namespace std;

/**
 * Resuelve el problema de la leche con mezcla utilizando algoritmos de busqueda local
 *
 * En este proyecto aborda el problema del enrutamiento de camiones para la recolección
 * de leche considerando la posibilidad de mezcla que recogen leche desde un conjunto
 * de granjas productoras y la llevan a una planta para su posterior tratamiento, teniendo
 * en cuenta que en cada granja se produce leche de distinto precio y calidad, y esta puede
 * ser mezclada tanto en la ruta como en la planta.
 *
 * @copyright  2017 Insive SpA
 * @version    Release: 1.0.37
 * @link       https://github.com/yorsh21/Proyecto-IA
 * @since      20 Julio del 2018
 */
class Solver {
	public:
		string name_instance;
		int trucks_lenght;
		int milks_lenght;
		int farms_lenght;
		int global_quality;
		int x_capacity;
		int x_request;
		int min_index_value;
		float disturbing;
		
		vector<float> milk_values;
		vector<int> plant_cuotes;
		vector<int> truck_capacities;
		vector<int> farms_types;
		vector<int> farms_milk;
		vector<vector<float>> farms_locates;
		vector<vector<int>> cost_matrix;
		vector<vector<int>> farms_by_milk;

		vector<float> result_times;
		vector<float> result_qualities;
		vector<int> global_solution;
		vector<int> quality_by_route;
		vector<int> pivots;
		vector<int> remaining_capacity;
		vector<int> satisfied_cuotes;


		Solver(Instances instance, string file_name, int x_cap, int x_req, float dist);

		//Evaluate
		int evaluate(vector<vector<int>> solution, bool show);
		int fast_evaluate(vector<int> row, int eval, int index1, int index2);


		//Local Search
		vector<vector<int>> hill_climbing(int end_time, int max_quality);
		vector<vector<int>> hill_climbing2(int end_time, int max_quality);
		vector<vector<int>> move_extra_routes(vector<vector<int>> solution, int index1, int index2, int node);
		vector<vector<int>> move_intra_routes(vector<vector<int>> solution, int row, int node1, int node2);
		vector<vector<int>> random_feasible_solution();
		vector<vector<int>> random_feasible_solution2();
		vector<vector<int>> disturbing_solution(vector<vector<int>> solution);

		vector<int> random_int_vector(int lenght);
		vector<int> clutter_vector(vector<int> array);

		bool feasible_movement(int index1, int index2);
		void vector_map_milk_types(vector<vector<int>> solution);
		void string_map_milk_types(vector<vector<int>> solution);


		//Utilities
		void print(int element);
		void print(float element);
		void print(string element);

		void print_vector(vector<int> array);
		void print_vector(vector<float> array);
		void print_vector(vector<string> array);

		void print_matrix(vector<vector<int>> matrix);
		void print_matrix(vector<vector<float>> matrix);
		void print_matrix(vector<vector<string>> matrix);

		string matrix_to_string(vector<vector<int>> array);
		string vector_to_string(vector<int> array);
		string time();


		//Exports
		void save_thread_result(string text);
		void export_solution(vector<vector<int>> solution);
};
