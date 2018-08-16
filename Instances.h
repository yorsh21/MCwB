#include <fstream>
#include <vector>
#include <tuple>

using namespace std;

class Instances {
	public:
		int truck_lenght;
		int milk_lenght;
		int nodes_lenght;
		vector<int> plant_cuotes;
		vector<int> truck_capacities;
		vector<float> milk_values;
		vector<vector<int>> farms_locates;

		void read_instances(string filename);
		void print_plant_cuotes();
		void print_truck_capacities();
		void print_milk_values();
		void print_farms_locates();
};