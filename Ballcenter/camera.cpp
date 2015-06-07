#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "circles.h"
#include "debug.h"
#include "const.h"
#include "camera.h"

using namespace cv;

bool Camera::open(int id){
	cam = VideoCapture(id);
	cam.set(CV_CAP_PROP_FRAME_HEIGHT, IN_HEIGHT);
	cam.set(CV_CAP_PROP_FRAME_WIDTH, IN_WIDTH);
	if (!cam.isOpened()){
		return 0;
	}
	else{
		return 1;
	}
}
void Camera::get_frame(){
	get_frame(true);
}
void Camera::get_frame(bool do_perspective){
	cam >> src; // raw frame
	//src = src(aoi); // area of interest
	if (do_perspective){
		warpPerspective(src, src, transmtx, Size(IN_WIDTH, IN_HEIGHT));
	}
}
void Camera::get_raw_frame(){
	cam >> src;
}
void Camera::set_perspective(std::vector<cv::Point>& corners){
	vector<Point2f> corners_float;
	for (auto &corner : corners) // access by reference to avoid copying
	{
		corners_float.push_back(Point2f(corner));
	}
	// output points
	std::vector<cv::Point2f> quad_pts;
	quad_pts.push_back(cv::Point2f(0, 0));
	quad_pts.push_back(cv::Point2f(IN_WIDTH, 0));
	quad_pts.push_back(cv::Point2f(IN_WIDTH, IN_HEIGHT));
	quad_pts.push_back(cv::Point2f(0,IN_HEIGHT));

	// Get mass center
	cv::Point2f center(0, 0);
	for (int i = 0; i < corners_float.size(); i++)
		center += corners_float[i];

	center *= (1. / corners_float.size());
	sortCorners(corners_float, center);
	// Get transformation matrix
	transmtx = cv::getPerspectiveTransform(corners_float, quad_pts);

}
void Camera::sortCorners(std::vector<cv::Point2f>& corners, cv::Point2f center)
{
	std::vector<cv::Point2f> top, bot;

	for (int i = 0; i < corners.size(); i++)
	{
		if (corners[i].y < center.y)
			top.push_back(corners[i]);
		else
			bot.push_back(corners[i]);
	}

	cv::Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
	cv::Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
	cv::Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
	cv::Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];

	corners.clear();
	corners.push_back(tl);
	corners.push_back(tr);
	corners.push_back(br);
	corners.push_back(bl);
}
