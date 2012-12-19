#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>
#include <fstream>

class Config {
		static std::map<std::string, std::string> config;

public:
		static void load(std::string file_name);
		static std::string get(std::string g);
};

#endif