#pragma once

namespace bc{
	static cv::VideoCapture cam;

	void find_circles(cv::Mat& src, cv::vector<cv::Vec<float, 3>>& circles);
	void draw_circles(cv::Mat& src, float x_scale, float y_scale, cv::vector<cv::Vec<float, 3>>& circles);
}
