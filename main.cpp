#include <iostream>
#include "Instances.cpp"
#include "Solution.cpp"

using namespace std;

int main(int argc, char *argv[]) {
	cout << "===========================" << endl;

	Instances instances = Instances();
	instances.read_instances("instances/a36.txt");

	//instances.print_plant_cuotes();
	//instances.print_truck_capacities();
	//instances.print_milk_values();
	//instances.print_farms_locates();

	Solution sol = Solution();
	sol.init(instances.truck_capacities, instances.milk_values, instances.farms_locates);
	sol.build();

	return 0;
}