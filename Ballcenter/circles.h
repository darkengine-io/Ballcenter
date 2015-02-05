#pragma once

namespace bc{
	static cv::VideoCapture cam;

	void find_circles(cv::Mat& src, cv::vector<cv::Vec<float, 3>>& circles);
	void draw_circles(cv::Mat& src, cv::vector<cv::Vec<float, 3>>& circles);
	void find_blobs(cv::Mat& src, std::vector<std::vector<cv::Point>>& blobs, std::vector<cv::Vec<int, 4>>& hierarchy);
	void draw_blobs(cv::Mat& src, std::vector<std::vector<cv::Point>>& blobs, std::vector<cv::Vec<int, 4>>& hierarchy);
	void out_create(cv::Mat& src);
	void redraw(cv::Mat& src);
}
