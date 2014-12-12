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
	namedWindow(OUT_WIN, CV_WINDOW_AUTOSIZE);
	while (true){
		fps_start("loop");
		cam.get_frame();
		
		bc::draw_circles(bc::find_circles(cam.src), cam.src, cam.x_scale, cam.y_scale);
		/// Draw the circles detected

		//imshow(MAIN_WIN, src);
		if (waitKey(1) >= 0) break;
		dp("--------------------");
	}
	return 0;
}
