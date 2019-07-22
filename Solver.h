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


		Solver(Instances instance, string file_name, int x_cap, int x_req);

		int evaluate(vector<int> solution, bool show);
		int fast_evaluate_2opt(vector<int> solution, int old_eval, int index1, int index2);
		int fast_evaluate_swap(vector<int> solution, int old_eval, int index1, int index2);
		int random_index(vector<int> solution);


		//Búsqueda Local
		vector<int> hill_climbing();
		//vector<int> hill_climbing(int end_time, int max_quality);
		vector<int> improve_solution(vector<int> solution, vector<int> trucks_order);
		vector<int> long_swap(vector<int> solution, int index1, int index2);
		vector<int> move_extra_routes(vector<int> solution, int index1, int index2);
		vector<int> two_opt(vector<int> solution, int index1, int index2);
		vector<int> neighbour_2opt_index(vector<int> solution, int index);
		vector<int> neighbour_move_index(vector<int> solution, int index);
		vector<int> random_feasible_solution();
		vector<int> random_feasible_solution2();
		vector<int> random_solution();
		vector<int> get_node_from_route(vector<int> solution, int number);
		vector<int> random_int_vector(int lenght);
		vector<int> random_assignment(vector<int> array);
		vector<vector<int>> split_routes(vector<int> solution);
		vector<vector<int>> split_route(vector<int> solution, int number);
		vector<vector<int>> intelligence_split_route(vector<int> solution, int index);
		bool can_move_extra_routes(vector<int> solution, int index1, int index2);

		//Utilities
		void print(int element);
		void print(float element);
		void print(string element);

		void print_vector(vector<int> array);
		void print_vector(vector<float> array);
		void print_vector(vector<string> array);

		void print_farms_locates();
		void print_cost_matrix();
		string int_vector_to_string(vector<int> array);
		string time();

		//Exports
		void save_row_result();
		void save_thread_result(string text);
		void draw_graph(vector<int> solution, int quality);
};
