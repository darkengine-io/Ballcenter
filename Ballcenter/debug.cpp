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

float timer_end(std::string comment, bool print){
	float time = (clock() - start_times[comment]) / (float)CLOCKS_PER_SEC;
	if (print){
		std::cout << comment + ": " + std::to_string(time * 1000) + " ms" << std::endl;
	}
	return time;
}

float fps_end(std::string comment){
	return 1 / timer_end(comment, false);
}