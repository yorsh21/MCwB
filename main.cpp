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

vector<int> seeds = {
	1437356881, 1437356667, 1437356643, 1437356562, 1437356443, 1437356427, 1437356575, 1437356478, 1437356067, 143735625,
	1192356381, 1192356367, 1192356343, 1192356362, 1192356343, 1192356327, 1192356375, 1192356378, 1192356367, 119235635,
	1283156388, 1283156047, 1283156327, 1283156012, 1283156343, 1283150322, 1280156375, 1283106375, 1283156364, 128315039
};

vector<int> times = {
	62,40,110,45,570,110,101,136,66,230,183,304,270,3565,561,1022,2930,5395,478,1552,5626,84,13760,4806,15056,4537,
	2645,80,3483,3659,12,6,99,58,154,1700,284,5959
};

vector<int> qualities = {
	29417,30496,29233,24837,28596,30808,38771,40282,40696,39800,46662,22414,24694,25041,60644,22917,24447,24100,28046,
	25822,29977,66160,46180,42992,48238,25906,43869,23705,72864,72072,15947,7207,7117,20409,50128,91461,17452,13173
};

vector<string> inputs = {
	"a33","a34","a36","a37","a38","a39","a44","a45","a46","a48","a53","a54","a55","a60","a61","a62","a63","a64","a65",
	"a69","a80","c50","c75","tai75A","tai75B","tai75C","tai75D","f45","f71","f72","eil22","eil23","eil30","eil33",
	"eil51","eil76","att48","_real"
};


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
	else if(argc == 2) {
		string input = argv[1];
		int seed = time(NULL);//seeds[0];
		int time = 0;
		int quality = 0;
		int factor1 = 10;
		int factor2 = 10;

		for (int i = 0; i < (int)inputs.size(); ++i)
		{
			if(inputs[i] == input) {
				time = times[i];
				quality = qualities[i];
			}
		}

		srand (seed);
		
		//Leyendo instancias
		Instances instance = Instances();
		if(!instance.read_instances(input + ".txt"))
			return 0;

		//Creando estructura de las soluciones
		Solver sol = Solver(instance, input, factor1, factor2);


		vector<vector<int>> rand_sol = sol.random_feasible_solution2();
		//sol.string_map_milk_types(rand_sol);
		//return 0;

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
