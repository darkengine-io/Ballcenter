#pragma once
#include <vector>
#include <iostream>
#include <string>

typedef struct {
	cv::Point center;
	int color;
} square;

namespace legos{
	static int x, y, width, height;
	square pass(cv::Mat& image, int Count);
	void legos(Camera& cam, int * red, int * green, int * blue);
	static std::vector<std::vector<cv::Point>> squares_g;
}

