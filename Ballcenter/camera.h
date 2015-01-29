#pragma once

class Camera{
public:
	cv::VideoCapture cam;
	void get_frame();
	bool open(int id);
	void get_raw_frame();
	cv::Rect aoi;
	cv::Mat src;
};