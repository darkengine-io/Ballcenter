#pragma once

namespace bc{
	static cv::VideoCapture cam;
	static cv::vector<cv::Vec<float, 3>>* circles_p;
	static cv::vector<cv::Vec<float, 3>> circles;

	cv::vector<cv::Vec<float, 3>>* find_circles(cv::Mat& src);
	void draw_circles(cv::Mat& src, float x_scale, float y_scale);
}