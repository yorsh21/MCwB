#include <iostream>
#include <time.h>
#include "Instances.cpp"
#include "Solution.cpp"
#include "Solver.cpp"

using namespace std;

int main(int argc, char *argv[]) {
	//Set random Seed
	srand (time(NULL));

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
	sol.hill_climbing(10000);

	//Solver hill_climbing = Solver();
	//hill_climbing.init(sol);
	//hill_climbing.run();

	return 0;
}