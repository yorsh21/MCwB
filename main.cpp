#include <iostream>
#include <time.h>
#include "Instances.cpp"
#include "Solver.cpp"

using namespace std;

int main(int argc, char *argv[]) {
	cout << "==========================================" << endl;
	if(argc == 1){
		cout << "The algoritm needs instance filename, for example instance=myinstance.txt open file: inputs/instance.txt " << endl;
		return 0;
	}

	//Set random Seed
	int seed = time(NULL);
	srand (seed);

	cout << "Random Seed: " << seed  << endl << endl;

	//Leyendo instancias
	Instances instances = Instances();
	instances.read_instances(argv[1]);

	//Creando estructura de las soluciones
	Solver sol = Solver();
	sol.init(instances.truck_capacities, instances.milk_values, instances.farms_locates, instances.plant_cuotes);
	
	//Ejecutando algoritmo de búsqueda local
	vector<int> solution = sol.hill_climbing(1000);

	//Exportando solución
	sol.export_result(solution, argv[1]);

	return 0;
}