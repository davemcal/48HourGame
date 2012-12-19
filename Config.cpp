#include "Config.h"

#include <algorithm>

std::map<std::string, std::string> Config::config;

void Config::load(std::string file_name) {
	std::ifstream in(file_name.c_str());

	std::string a, b;
	while (in >> a) {
		getline(in, b);
		b.erase(b.begin(), b.begin() + b.find_first_not_of(" "));
		b.erase(b.find_last_not_of(" ") + 1);

		config[a] = b;
	}
}

std::string Config::get(std::string g) {
	return config[g];
}