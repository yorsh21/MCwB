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

void pruebas(string filename, vector<int> solution, vector<int> trucks) {
	Instances instance = Instances();
	instance.read_instances(filename + ".txt");

	srand ((int)time(NULL));

	Solver sol = Solver(instance, filename);

	sol.truck_capacities = trucks;
	sol.evaluate(solution, true);


	/*for (int i = 0; i < 100000; ++i)
	{
		vector<int> solution = sol.random_feasible_solution();
		sol.print_int_vector(solution);
		sol.print(sol.evaluate(solution));
	}*/
}

void mejoramiento(string filename) {
	Instances instance = Instances();
	instance.read_instances(filename + ".txt");

	srand ((int)time(NULL));

	Solver sol = Solver(instance, filename);

	vector<int> solution = {0,13,46,19,7,43,37,1,22,16,40,28,25,4,34,10,31,0,14,23,11,32,35,29,26,17,20,38,41,2,5,47,0,44,24,21,12,15,45,42,36,6,27,18,30,8,9,39,3,33,0};
	vector<int> trucks_order = {40000,50000,45000};

	sol.improve_solution(solution, trucks_order);
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
		if(!instance.read_instances(input + ".txt"))
			return 0;

		//Creando estructura de las soluciones
		Solver sol = Solver(instance, input);

		//Ejecutando algoritmo de búsqueda local
		vector<int> solution = sol.hill_climbing(time, quality);
		//int eval = sol.evaluate(solution);

		//cout << "::::::::::::::::::: END: " + input + " -> " + to_string(eval) + " :::::::::::::::::::" << endl;
		return 0;
	}
	else {
		//string filename = "f45";
		//vector<int> solution = {0,24,21,18,12,3,6,27,33,30,39,36,42,15,9,0,1,37,40,34,4,7,31,28,43,19,22,25,10,16,0,20,23,26,8,44,41,32,29,5,35,38,14,13,11,17,2,0};
		//vector<int> trucks = {15000,10000,20000};

		//pruebas(filename, solution, trucks);
		return 0;

	}
}