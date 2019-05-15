#include <iostream>
#include <time.h>
#include <string>
#include <iterator>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <ctime>
#include <thread>
#include <chrono>
#include <thread>
#include <mutex>
#include <experimental/filesystem>

#include "Instances.cpp"
#include "Solver.cpp"

using namespace std;
namespace fs = std::experimental::filesystem;

void pruebas() {
	Instances instance = Instances();
	instance.read_instances("a36.txt");

	srand ((int)time(NULL));

	Solver sol = Solver(instance.truck_capacities, instance.milk_values, instance.farms_locates, instance.plant_cuotes, "a36");

	for (int i = 0; i < 100000; ++i)
	{
		vector<int> solution = sol.random_feasible_solution();
		sol.print_int_vector(solution);
		sol.print(sol.evaluate(solution));
	}
}

void mejoramiento() {
	Instances instance = Instances();
	instance.read_instances("a64.txt");

	srand ((int)time(NULL));

	Solver sol = Solver(instance.truck_capacities, instance.milk_values, instance.farms_locates, instance.plant_cuotes, "a64");

	vector<int> solution = {0,29,11,17,14,59,5,23,62,20,32,56,41,0,6,15,45,33,48,42,51,9,24,60,8,47,63,2,54,50,36,39,44,21,26,35,27,30,3,53,57,12,18,38,0,10,52,13,61,16,46,58,4,34,19,37,31,55,49,40,7,1,28,43,22,25,0};

	sol.improve_solution(solution);
}


int main(int argc, char *argv[]) 
{
	if(argc == 5) 
	{
		string input = argv[1];
		int seed = stoi(argv[2]);
		int time = stoi(argv[3]);
		int quality = stoi(argv[4]);

		srand (seed);
		
		//Leyendo instancias
		Instances instance = Instances();
		instance.read_instances(input + ".txt");

		//Creando estructura de las soluciones
		Solver sol = Solver(instance.truck_capacities, instance.milk_values, instance.farms_locates, instance.plant_cuotes, input);

		//Ejecutando algoritmo de búsqueda local
		vector<int> solution = sol.hill_climbing(time, quality);
		//int eval = sol.evaluate(solution);

		//cout << "::::::::::::::::::: END: " + input + " -> " + to_string(eval) + " :::::::::::::::::::" << endl;
		return 0;
	}
	else {
		//mejoramiento();
		return 0;
	}
}