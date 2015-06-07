#pragma once

class Camera{
public:
	cv::VideoCapture cam;
	void get_frame();
	void get_frame(bool do_perspective);
	void set_perspective(std::vector<cv::Point>& corners);
	bool open(int id);
	void get_raw_frame();
	cv::Rect aoi;
	cv::Mat src;
private:
	cv::Mat transmtx;
	void sortCorners(std::vector<cv::Point2f>& corners, cv::Point2f center);
};