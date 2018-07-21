#include <string>
#include <iterator>
#include <sstream>
#include "Instances.h"

void Instances::read_instances(string filename) {
	
	ifstream myfile;
	myfile.open (filename);

	string string_lenght;
	string string_cuotes;
	string string_milk;
	string string_locate;

	if (myfile.is_open()) {
		getline (myfile, string_lenght);
		getline (myfile, string_cuotes);
		getline (myfile, string_milk);

		lenght = stoi(string_lenght);

		istringstream iss_cuot(string_cuotes);
		vector<int> cuot((istream_iterator<int>(iss_cuot)), istream_iterator<int>());
		cuotes = cuot;

		istringstream iss_milk(string_milk);
		vector<string> milk((istream_iterator<string>(iss_milk)), istream_iterator<string>());
		for (int i = 0; i < (int)milk.size(); ++i)
		{
			int value = stoi(milk[i].substr(0, 3));
			string type = milk[i].substr(3);

			milk_values.push_back(value);
			milk_types.push_back(type);
		}
		

		for (int i = 0; i < lenght; ++i)
		{
			getline (myfile, string_locate);

			istringstream iss_locate(string_locate);
			vector<float> locate((istream_iterator<float>(iss_locate)), istream_iterator<float>());

			tuple<float, float> coordenates = {locate[0], locate [1]};
			locates.push_back(coordenates);
		}

		/*
		cout << lenght << endl;
		cout << cuotes[0] << endl;
		cout << cuotes[1] << endl;
		cout << cuotes[2] << endl;
		*/
	}
	myfile.close();
}


void Instances::print_cuotes() {
	cout << "[";
	for (int i = 0; i < (int)cuotes.size() - 1; ++i)
	{
		cout << cuotes[i] << ", ";
	}
	cout << cuotes[cuotes.size()-1] << "]" << endl;
}

void Instances::print_milk_values() {
	cout << "[";
	for (int i = 0; i < (int)milk_values.size() - 1; ++i)
	{
		cout << milk_values[i] << ", ";
	}
	cout << milk_values[milk_values.size()-1] << "]" << endl;
}

void Instances::print_milk_types() {
	cout << "[";
	for (int i = 0; i < (int)milk_types.size() - 1; ++i)
	{
		cout << milk_types[i] << ", ";
	}
	cout << milk_types[milk_types.size()-1] << "]" << endl;
}

void Instances::print_locates() {
	cout << "[";
	for (int i = 0; i < (int)locates.size() - 1; ++i)
	{
		cout << "(" << get<0>(locates[i]) << ", " <<  get<1>(locates[i]) <<  ")" << ", ";
	}
	cout << "(" << get<0>(locates[(int)locates.size()-1]) << ", " <<  get<1>(locates[(int)locates.size()-1]) <<  ")" << "]" << endl;
}
