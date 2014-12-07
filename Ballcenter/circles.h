#ifndef CIRCLES_H
#define CIRCLES_H
namespace bc{
	static cv::VideoCapture cam;
	bool open(int id);
	void get_frame();
	cv::vector<cv::Vec<float, 3>> find_circles();
	void draw_circles(std::vector<cv::Vec<float, 3>> circles);
}
#endif