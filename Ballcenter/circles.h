#pragma once

namespace bc{
	static cv::VideoCapture cam;
	static cv::vector<cv::Vec<float, 3>> circles;
	void find_circles(cv::Mat& src);
	void draw_circles(cv::Mat& src, float x_scale, float y_scale);
}