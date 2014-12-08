#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "const.h"
#include "debug.h"
#include "circles.h"

using namespace cv;

int main(int argc, char** argv)
{
	dp("Starting");
	
	dp("Setting up capture");
	bc::open(0);


	dp("Opening windows");
	namedWindow(MAIN_WIN, CV_WINDOW_AUTOSIZE);
	namedWindow(OUT_WIN, CV_WINDOW_AUTOSIZE);
	while (true){
		ts("loop");
		bc::get_frame();
		
		bc::draw_circles(bc::find_circles());
		/// Draw the circles detected

		//imshow(MAIN_WIN, src);
		if (waitKey(1) >= 0) break;
		te("loop");
		dp("--------------------");
	}
	return 0;
}
