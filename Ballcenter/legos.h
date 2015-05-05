#pragma once
#include <vector>
#include <iostream>
#include <string>

namespace legos{
	static int x, y, width, height;
	cv::Point pass(int Color, int Count);
	void legos(Camera& cam, int * red, int * green, int * blue);
}