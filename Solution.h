using namespace std;

class Solution {
	public:
		int truck_lenght;
		int farm_lenght;
		vector<int> representation;
		vector<int> truck_capacities;
		vector<float> milk_values;
		vector<vector<int>> farms_locates;

		void init(vector<int> capacities, vector<float> values, vector<vector<int>> locates);
		void build();
		float evaluate(vector<int> solution);
		void detail_evaluate(vector<int> solution);
		void myopic();
		void move(int index1, int index2);


		//Búsqueda Local
		void hill_climbing(int restarts);
		vector<int> neighbour(vector<int> solution, int identity);
		vector<int> random_solution();


		//Find Nodes
		int nearest_node(int node);
		int nearest_node_same_milk(int node, int milk);
		vector<int> current_truck_capacity();


		//Utilities
		void print_int_vector(vector<int> array);
		void print_farms_locates();
		float distance_between_nodes(int farm1, int farm2);
		bool same_milk(int farm1, int farm2);
};