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
	if(argc == 7) 
	{
		string input = argv[1];
		int seed = stoi(argv[2]);
		int time = stoi(argv[3]);
		int quality = stoi(argv[4]);
		int factor1 = stoi(argv[5]);
		int factor2 = stoi(argv[6]);

		srand (seed);
		
		//Leyendo instancias
		Instances instance = Instances();
		if(!instance.read_instances(input + ".txt"))
			return 0;

		//Creando estructura de las soluciones
		Solver sol = Solver(instance, input, factor1, factor2);

		//Ejecutando algoritmo de búsqueda local
		vector<vector<int>> solution = sol.hill_climbing(time, quality);
		int eval = sol.evaluate(solution);

		cout << "::::::::::::::::::: END: " + input + " -> " + to_string(eval) + " :::::::::::::::::::" << endl;
		return 0;
	}
	else {
		return 0;
	}
}
