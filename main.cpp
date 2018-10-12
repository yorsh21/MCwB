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
#include <experimental/filesystem>

#include "Instances.cpp"
#include "Solver.cpp"

using namespace std;
namespace fs = std::experimental::filesystem;

int main(int argc, char *argv[]) {
	cout << "==========================================" << endl;

	vector<string> inputs;
	if(argc == 1) {//No se introdujo el nombre de la instancia
		string path = "inputs";
	    for (auto & p : fs::directory_iterator(path)) {
	        inputs.push_back(p.path().filename());
	    }
	}
	else {
		inputs.push_back(argv[1]);
	}


	//Set random Seed
	int seed = time(NULL);
	srand (seed);

	cout << "Random Seed: " << seed  << endl << endl;

	for (vector<string>::iterator input = inputs.begin(); input != inputs.end(); ++input)
	{
		//Leyendo instancias
		Instances instances = Instances();
		instances.read_instances(*input);

		//Creando estructura de las soluciones
		Solver sol = Solver();
		sol.init(instances.truck_capacities, instances.milk_values, instances.farms_locates, instances.plant_cuotes);
		
		//Ejecutando algoritmo de búsqueda local
		vector<int> solution = sol.hill_climbing(100);
		
		//Exportando solución
		sol.save_row_result(solution, *input);
		//sol.export_result(solution, *input);
	}


	return 0;
}