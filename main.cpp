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


int main(int argc, char *argv[])
{
	string input = "_real";
	int factor1 = 10;
	int factor2 = 10;
	int seed = 1437356881;

	srand (seed);

	//Leyendo instancias
	Instances instance = Instances();
	if(!instance.read_instances(input + ".txt"))
		return 0;

	//Creando estructura de las soluciones
	Solver sol = Solver(instance, input, factor1, factor2);

	//vector<vector<int>> solution = sol.random_feasible_solution();
	//vector<vector<int>> solution2 = sol.move_extra_routes(solution, 0, 12, 0);

	//sol.print_matrix(solution);
	//sol.print_matrix(solution2);
	//sol.evaluate(solution, true);

	//Ejecutando algoritmo de búsqueda local
	vector<vector<int>> solution = sol.hill_climbing(1);

	return 0;

}
