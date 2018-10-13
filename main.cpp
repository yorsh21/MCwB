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

void clean_result() {
	string file = "outputs/results.out";
	ofstream myfile;
	myfile.open (file);
	myfile.close();
}

int main(int argc, char *argv[]) {
	cout << "==========================================" << endl;

	if(argc == 1) {
		vector<string> inputs;
		vector<int> seeds = {1539354881, 1539354669, 1539354643, 1539354562, 1539354443, 1539354427, 1539353575, 1539352478, 1539352067, 1539352067};

	    for (auto & p : fs::directory_iterator("inputs")) {
	        inputs.push_back(p.path().filename());
	    }
		sort(inputs.begin(), inputs.end());
		clean_result();

		for (vector<string>::iterator input = inputs.begin(); input != inputs.end(); ++input) {
			//Leyendo instancias
			Instances instances = Instances();
			instances.read_instances(*input);

			//Creando estructura de las soluciones
			Solver sol = Solver();
			sol.init(instances.truck_capacities, instances.milk_values, instances.farms_locates, instances.plant_cuotes);
			
			//Ejecutando algoritmo de búsqueda local
			for (int i = 0; i < (int)seeds.size(); ++i)
			{
				srand (seeds[i]);
				vector<int> solution = sol.hill_climbing(1000);
			}
			
			//Exportando solución
			sol.save_row_result(*input);
		}
	}
	else {
		int seed = time(NULL);
		srand (seed);
		string input = argv[1];

		cout << "Random Seed: " << seed  << endl << endl;

		//Leyendo instancias
		Instances instances = Instances();
		instances.read_instances(input);

		//Creando estructura de las soluciones
		Solver sol = Solver();
		sol.init(instances.truck_capacities, instances.milk_values, instances.farms_locates, instances.plant_cuotes);
		
		//Ejecutando algoritmo de búsqueda local
		vector<int> solution = sol.hill_climbing(100);
		
		//Exportando solución
		sol.export_result(solution, input);
	}
	
	return 0;
}