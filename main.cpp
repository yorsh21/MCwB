#include <iostream>
#include "Instances.cpp"

using namespace std;

int main() {
	cout << "===========================" << endl;

	Instances instances = Instances();
	instances.read_instances("instances.txt");

	Map map = Map();

	Solver solve = Solver();


	instances.print_cuotes();
	instances.print_milk_values();
	instances.print_milk_types();
	instances.print_locates();
	return 0;
}