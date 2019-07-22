#include "Instances.h"

bool Instances::read_instances(string filename)
{
	string file = "inputs/" + filename;
	//cout << "Reading instances for: " << file << endl;

	ifstream myfile;
	myfile.open (file);

	if (myfile.is_open()) {
		string string_truck_lenght;
		string string_truck_capacities;
		string string_blank;
		string string_milk_lenght;
		string string_plant_cuotes;
		string string_milk_value;
		string string_farms_locates;
		string string_nodes_lenght;
		vector<string> letters = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"};

		getline (myfile, string_truck_lenght);
		getline (myfile, string_truck_capacities);
		getline (myfile, string_blank);
		getline (myfile, string_milk_lenght);
		getline (myfile, string_plant_cuotes);
		getline (myfile, string_milk_value);
		getline (myfile, string_blank);
		getline (myfile, string_nodes_lenght);

		truck_lenght = stoi(string_truck_lenght);
		milk_lenght = stoi(string_milk_lenght);
		nodes_lenght = stoi(string_nodes_lenght);

		istringstream iss_cap(string_truck_capacities);
		vector<int> cap((istream_iterator<int>(iss_cap)), istream_iterator<int>());
		truck_capacities = cap;

		istringstream iss_cuot(string_plant_cuotes);
		vector<int> cuot((istream_iterator<int>(iss_cuot)), istream_iterator<int>());
		plant_cuotes = cuot;

		istringstream iss_milk(string_milk_value);
		vector<float> milk((istream_iterator<float>(iss_milk)), istream_iterator<float>());
		milk_values = milk;

		for (int i = 0; i < milk_lenght; ++i)
			farms_by_milk.push_back({});

		for (int i = 0; i < nodes_lenght; ++i)
		{
			getline (myfile, string_farms_locates);

			istringstream iss_locate(string_farms_locates);
			vector<string> locate((istream_iterator<string>(iss_locate)), istream_iterator<string>());

			int type = -1;
			for (int i = 0; i < (int)letters.size(); ++i)
			{
				if(letters[i].compare(locate[1]) == 0) {
					type = i;
					break;
				}
			}

			if (type != -1){
				farms_by_milk[type].push_back(i);
			}

			farms_types.push_back(type);
			farms_milk.push_back(stoi(locate[2]));
		}

		getline (myfile, string_blank);

		for (int i = 0; i < nodes_lenght; ++i)
		{
			getline (myfile, string_farms_locates);

			istringstream iss_locate(string_farms_locates);
			vector<float> locate((istream_iterator<float>(iss_locate)), istream_iterator<float>());

			int size_locate = (int)locate.size();
			vector<int> row(size_locate, 0);
			for (int j = 0; j < size_locate; ++j)
			{
				row[j] = locate[j] + 0.5;
			}

			cost_matrix.push_back(row);
		}

		cout << "Successfully read file: " << file << endl;

		myfile.close();
		return true;
	}
	else {
		cout << "Error when reading the file:" << file << endl;
		return false;
	}
}


void Instances::print_plant_cuotes()
{
	cout << "[";
	for (int i = 0; i < (int)plant_cuotes.size() - 1; ++i)
	{
		cout << plant_cuotes[i] << ", ";
	}
	cout << plant_cuotes[plant_cuotes.size()-1] << "]" << endl;
}


void Instances::print_truck_capacities()
{
	cout << "[";
	for (int i = 0; i < (int)truck_capacities.size() - 1; ++i)
	{
		cout << truck_capacities[i] << ", ";
	}
	cout << truck_capacities[truck_capacities.size()-1] << "]" << endl;
}


void Instances::print_milk_values()
{
	cout << "[";
	for (int i = 0; i < (int)milk_values.size() - 1; ++i)
	{
		cout << milk_values[i] << ", ";
	}
	cout << milk_values[milk_values.size()-1] << "]" << endl;
}


void Instances::print_farms_locates()
{
	cout << "[";
	for (int i = 0; i < (int)farms_locates.size() - 1; ++i)
	{
		cout << "(" << farms_locates[i][0] << ", " <<  farms_locates[i][1] << ", "  <<  farms_locates[i][2] << ", " <<  farms_locates[i][3]  <<  "), ";
	}
	cout << "(" << farms_locates[(int)farms_locates.size()-1][0] << ", " <<  farms_locates[(int)farms_locates.size()-1][1] << ", " <<  farms_locates[(int)farms_locates.size()-1][2] << ", " <<  farms_locates[(int)farms_locates.size()-1][3] <<  ")" << "]" << endl;
}


void Instances::print_vector(vector<int> array)
{
	int len = (int)array.size();

	if (len == 0) {
		cout << "[]" << endl;
	}
	else {
		cout << "[";
		for (int i = 0; i < len - 1; ++i) {
			cout << array[i] << ",";
		}

		cout << array[len - 1] <<  "]" << endl;
	}
}


void Instances::print_vector(vector<float> array)
{
	int len = (int)array.size();

	if (len == 0) {
		cout << "[]" << endl;
	}
	else {
		cout << "[";
		for (int i = 0; i < len - 1; ++i) {
			cout << array[i] << ",";
		}

		cout << array[len - 1] <<  "]" << endl;
	}
}


void Instances::print_vector(vector<string> array)
{
	int len = (int)array.size();

	if (len == 0) {
		cout << "[]" << endl;
	}
	else {
		cout << "[";
		for (int i = 0; i < len - 1; ++i) {
			cout << array[i] << ",";
		}

		cout << array[len - 1] <<  "]" << endl;
	}
}
