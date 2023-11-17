#include <bits/stdc++.h>
#include <fstream>


using namespace std;

vector <pair<string,string>> data[10];

/**
 * @brief A class that stores data in a vector of pairs.
 */
class Data
{
public:
	vector <pair<string, string>> data[10];
	Data()
	{
		for (int i = 0; i < 10; ++i) {
			data[i].emplace_back("", "");
		}
	}
	void GetFileData()
	{
		ifstream inputFile("names.txt");

		if (!inputFile.is_open())
		{
			cerr << "ERROR OPENING FILE.........!!!!" << endl;
		}
		string line;
		while (getline(inputFile, line))
		{
			// Split the line into key and value using a delimiter (e.g., ":")
			std::size_t pos = line.find(":");
			if (pos != std::string::npos) {
				string key = line.substr(0, pos);
				string value = line.substr(pos + 1);

				int len = key.length();
				string num = line.substr(len - 2, pos);
				int val = stoi(num);
				// Create a key-value pair and add it to the vector
				std::pair<std::string, std::string> pair = std::make_pair(key, value);
				data[val/10].push_back(pair);
			}
		}
		inputFile.close();

	}
};


int main()
{
	Data d;
	d.GetFileData();
	for (int i = 0; i < 10; ++i) {
		for (auto it = d.data[i].begin(); it != d.data[i].end(); ++it) {
			cout << it->first << " " << it->second << endl;
		}
	}
	

for(int i = 0; i < 10; ++i) {
	cout << "Hello, world!" << endl;
}
return 0;
}
