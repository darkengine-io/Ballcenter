#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "const.h"
#include "debug.h"
#include "circles.h"
#include "camera.h"
#include "calibrate.h"
#include "data.h"

using namespace cv;

Camera cam;

int main(int argc, char** argv)
{
	dp("Starting");

	dp("Setting up capture");
	cam.open(0);
	
	dp("Calibrating");
	cal::calibrate(cam);

	dp("Opening windows");
	namedWindow(MAIN_WIN, CV_WINDOW_AUTOSIZE);
	namedWindow(OUT_WIN, CV_WINDOW_NORMAL);
	cvSetWindowProperty(OUT_WIN, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	while (true){
		fps_start("loop");
		cam.get_frame();
		
		bc::circles_p = bc::find_circles(cam.src);
		bc::draw_circles(cam.src, cam.x_scale, cam.y_scale);
		data::tick(*bc::circles_p);
		/// Draw the circles detected

		//imshow(MAIN_WIN, src);
		if (waitKey(1) >= 0) break;
		dp("--------------------");
	}
	return 0;
}
