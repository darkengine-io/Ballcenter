#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <time.h>

using namespace std;
using namespace cv;

Point center;

namespace
{
	Point last_position;
	float velocity(Point current_position){
		float vel = sqrt(pow(current_position.x - last_position.x, 2) + pow(current_position.y - last_position.y, 2));
		last_position = current_position;
		return vel;
	}
}

namespace
{
	// windows and trackbars name
	const std::string windowName = "Ball tracker";
	const std::string cannyThresholdTrackbarName = "Canny threshold";
	const std::string accumulatorThresholdTrackbarName = "Accumulator Threshold";

	// initial and max values of the parameters of interests.
	const int cannyThresholdInitialValue = 110;
	const int accumulatorThresholdInitialValue = 70;
	const int maxAccumulatorThreshold = 200;
	const int maxCannyThreshold = 255;

	void HoughDetection(const Mat& src_gray, const Mat& src_display, int cannyThreshold, int accumulatorThreshold)
	{
		// will hold the results of the detection
		std::vector<Vec3f> circles;
		// runs the actual detection
		HoughCircles(src_gray, circles, HOUGH_GRADIENT, 1, src_gray.rows / 8, cannyThreshold, accumulatorThreshold, 0, 0);

		// clone the colour, input image for displaying purposes
		Mat display = src_display.clone();
		for (size_t i = 0; i < circles.size(); i++)
		{
			center = Point(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// circle center
			circle(display, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			// circle outline
			circle(display, center, radius, Scalar(0, 0, 255), 3, 8, 0);
		}

		// shows the results
		putText(display, to_string(velocity(center)), center, FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0, 255, 100));
		imshow(windowName, display);
	}
}

int main(int argc, char** argv)
{
	VideoCapture cap(0);
	if (!cap.isOpened()){
		return -1;
	}
	Mat src, src_gray;
	cap >> src;

	//declare and initialize both parameters that are subjects to change
	int cannyThreshold = cannyThresholdInitialValue;
	int accumulatorThreshold = accumulatorThresholdInitialValue;

	// create the main window, and attach the trackbars
	namedWindow(windowName, WINDOW_AUTOSIZE);
	createTrackbar(cannyThresholdTrackbarName, windowName, &cannyThreshold, maxCannyThreshold);
	createTrackbar(accumulatorThresholdTrackbarName, windowName, &accumulatorThreshold, maxAccumulatorThreshold);

	// infinite loop to display
	// and refresh the content of the output image
	// until the user presses q or Q
	int key = 0;
	while (key != 'q' && key != 'Q')
	{
		cap >> src;
		// Convert it to gray
		cvtColor(src, src_gray, COLOR_BGR2GRAY);

		// Reduce the noise so we avoid false circle detection
		GaussianBlur(src_gray, src_gray, Size(9, 9), 2, 2);
		// those paramaters cannot be =0
		// so we must check here
		cannyThreshold = std::max(cannyThreshold, 1);
		accumulatorThreshold = std::max(accumulatorThreshold, 1);

		//runs the detection, and update the display
		HoughDetection(src_gray, src, cannyThreshold, accumulatorThreshold);

		// get user key
		key = waitKey(10);
	}

	return 0;
}