#include <iostream>
#include <string>
#include <map>
#include <ctime>
#include "debug.h"

std::map<std::string, std::clock_t> start_times;

bool dprint(std::string out){
	std::cout << out << std::endl;
	return false;
}

void timer_start(std::string comment){
	start_times[comment] = clock();
}

float timer_end(std::string comment){
	float time = (clock() - start_times[comment]) / (float)CLOCKS_PER_SEC;
	std::cout << comment + ": " + std::to_string(time) << std::endl;
	return time;
}