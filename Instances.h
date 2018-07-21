#include <fstream>
#include <vector>
#include <tuple>

using namespace std;

class Instances {
	public:
		int lenght;
		vector<int> cuotes;
		vector<int> milk_values;
		vector<string> milk_types;
		vector<tuple<float, float>> locates;

		void read_instances(string filename);
		void print_cuotes();
		void print_milk_values();
		void print_milk_types();
		void print_locates();
};