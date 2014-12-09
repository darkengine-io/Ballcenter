#pragma once

namespace bc{
	static cv::VideoCapture cam;
	cv::vector<cv::Vec<float, 3>> find_circles(cv::Mat src);
	void draw_circles(std::vector<cv::Vec<float, 3>> circles, cv::Mat src, float x_scale, float y_scale);
}