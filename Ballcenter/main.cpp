#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "const.h"

using namespace cv;

int main(int argc, char** argv)
{
	Mat src, src_gray;

	VideoCapture cam(0);
	cam.set(CV_CAP_PROP_FRAME_HEIGHT, 1024);
	cam.set(CV_CAP_PROP_FRAME_WIDTH, 768);

	namedWindow(MAIN_WIN, CV_WINDOW_AUTOSIZE);
	while (true){
		cam >> src;
		/// Convert it to gray
		cvtColor(src, src_gray, CV_BGR2GRAY);

		/// Reduce the noise so we avoid false circle detection
		GaussianBlur(src_gray, src_gray, Size(9, 9), 2, 2);

		vector<Vec3f> circles;

		/// Apply the Hough Transform to find the circles
		HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT, 1, MIN_DIST, UPPER_EDGE, CENTER_THRESH, MIN_RADIUS, MAX_RADIUS);

		/// Draw the circles detected
		for (size_t i = 0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// circle center
			circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			// circle outline
			circle(src, center, radius, Scalar(0, 0, 255), 3, 8, 0);
		}
		imshow(MAIN_WIN, src);
		if (waitKey(1) >= 0) break;
	}
	return 0;
}
