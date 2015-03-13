#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "const.h"
#include "debug.h"
#include "circles.h"
#include "camera.h"
#include "calibrate.h"
#include "legos.h"
#include "data.h"

using namespace cv;

cv::vector<cv::Vec<float, 3>> circles;
vector<vector<Point>> blobs;
vector<Vec4i> hierarchy;
Camera cam;

int main(int argc, char** argv)
{
	dp("Starting");

	dp("Setting up capture");
	cam.open(0);
	
	dp("Calibrating");
	cal::calibrate(cam);

	dp("Legos");
	legos::legos(cam);

	dp("Opening windows");
	namedWindow(MAIN_WIN, CV_WINDOW_AUTOSIZE);
	namedWindow(CAM_WIN, CV_WINDOW_AUTOSIZE);
	namedWindow(OUT_WIN, CV_WINDOW_NORMAL);
	cvSetWindowProperty(OUT_WIN, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	while (true){
		fps_start("loop");
		cam.get_frame();
		
		bc::out_create(cam.src);
		bc::redraw(cam.src);
		/// Draw the circles detected

		//imshow(MAIN_WIN, src);
		if (waitKey(1) >= 0) break;
		dp("--------------------");
	}
	return 0;
}
