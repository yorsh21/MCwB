#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <ctime>
#include "Instances.cpp"
#include "Solver.cpp"

using namespace std;

int main(int argc, char *argv[]) {
	cout << "==========================================" << endl;
	/*if(argc == 1){
		cout << "The algoritm needs instance filename, for example instance=myinstance.txt open file: inputs/instance.txt " << endl;
		return 0;
	}*/

	//Set random Seed
	int seed = time(NULL);
	srand (seed);

	cout << "Random Seed: " << seed  << endl << endl;

	//Vector de Instancias
	vector<string> inputs = {"a36.txt", "a62.txt", "a64.txt", "a65.txt", "eil22.txt", "eil51.txt", "eil76.txt", "tai75A.txt"};

	for (int i = 0; i < (int)inputs.size(); ++i)
	{
		//Leyendo instancias
		Instances instances = Instances();
		instances.read_instances(inputs[i]);
		//instances.read_instances(argv[1]);

		//Creando estructura de las soluciones
		Solver sol = Solver();
		sol.init(instances.truck_capacities, instances.milk_values, instances.farms_locates, instances.plant_cuotes);
		
		//Ejecutando algoritmo de búsqueda local
		vector<int> solution = sol.hill_climbing(10000);

		//Exportando solución
		//sol.export_result(solution, argv[1]);
	}
	

	return 0;
}