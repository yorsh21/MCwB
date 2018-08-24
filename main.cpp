#include <iostream>
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

	//Leyendo instancias
	Instances instances = Instances();
	instances.read_instances("a36.txt");
	//instances.read_instances(argv[1]);

	//Creando estructura de las soluciones
	Solver sol = Solver();
	sol.init(instances.truck_capacities, instances.milk_values, instances.farms_locates, instances.plant_cuotes);
	
	//Ejecutando algoritmo de búsqueda local
	clock_t startTime = clock();
	vector<int> solution = sol.hill_climbing(1000);
	cout << "\nTiempo total de Ejecución: " << (clock() - startTime) / CLOCKS_PER_SEC << endl;

	//Exportando solución
	//sol.export_result(solution, argv[1]);

	return 0;
}