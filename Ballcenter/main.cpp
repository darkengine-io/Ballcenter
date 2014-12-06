#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <time.h>
#include <ctime>

using namespace std;
using namespace cv;

bool debug = true;
Point center;
Mat display;
Mat output;
Mat white;
float Vel = 0;
float radius = 0;
namespace
{
	std::vector<Point> current_positions;
	std::vector<Point> last_positions;
	//std::vector<float> velocities;
	std::vector<float> velocity(){
		std::vector<float> velocities;
		int blah = current_positions.size();
		for (int i = 0; i < min(current_positions.size(), last_positions.size()); i++){
			velocities.push_back(sqrt(pow(current_positions[i].x - last_positions[i].x, 2) + pow(current_positions[i].y - last_positions[i].y, 2)));
			Vel = sqrt(pow(current_positions[i].x - last_positions[i].x, 2) + pow(current_positions[i].y - last_positions[i].y, 2));
			putText(display, to_string(Vel), current_positions[i], FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0, 255, 100));
		}
		//float vel = sqrt(pow(current_position.x - last_position.x, 2) + pow(current_position.y - last_position.y, 2));
		//last_position = current_position;
		last_positions = current_positions;
		current_positions.clear();
		return velocities;
	}
}

namespace
{
	// windows and trackbars name
	const std::string windowName = "Ball tracker";
	const std::string cannyThresholdTrackbarName = "Canny threshold";
	const std::string accumulatorThresholdTrackbarName = "Accumulator Threshold";

	// initial and max values of the parameters of interests.
	const int cannyThresholdInitialValue = 110;  //Initial value was 110
	const int accumulatorThresholdInitialValue = 30; //Initial value was 70
	const int maxAccumulatorThreshold = 200;
	const int maxCannyThreshold = 255;

	

	void HoughDetection(const Mat& src_gray, const Mat& src_display, int cannyThreshold, int accumulatorThreshold)
	{
				//Time start
				std::clock_t    start;

				output = white.clone();
		// will hold the results of the detection
		std::vector<Vec3f> circles;
		std::vector<Vec3f> lines;

				//Time start
				start = 0;
				start = std::clock();


		// runs the actual detection
		HoughCircles(src_gray, circles, HOUGH_GRADIENT, 1, src_gray.rows / 8, cannyThreshold, accumulatorThreshold, 0, 0);
	//	HoughLines(src_gray, lines, HOUGH_GRADIENT, 1, src_gray.rows / 8, cannyThreshold, accumulatorThreshold, 0, 0);


				std::cout << ">Detection of object in Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
				//End Time

				//Time start
				start = 0;
				start = std::clock();

		// clone the colour, input image for displaying purposes
		display = src_display.clone();
		//output = src_display.clone();


				std::cout << ">Display outputed in Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
				//End Time

				//Time start
				start = 0;
				start = std::clock();

		for (size_t i = 0; i < circles.size(); i++)
		{
			center = Point(cvRound(circles[i][0]), cvRound(circles[i][1]));
			current_positions.push_back(center);
			radius = cvRound(circles[i][2]);
			// circle center
			circle(output, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			//circle outline;
			circle(output, center, radius + 8 , Scalar(0, 100, 0), 3, 8, 0);

			// circle center
			circle(display, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			//circle outline;
			circle(display, center, radius, Scalar(0, 0, 255), 3, 8, 0);


		}
		

				std::cout << ">Circles drawn in Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
				//End Time

				//Time start
				start = 0;
				start = std::clock();


		// shows the results
		//putText(display, to_string(velocity(center)), center, FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0, 255, 100));
		velocity();

				std::cout << ">Velocity Calculated in Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
				//End Time

		
		//TEST CODE : Behnam Saeedi(Ben)
		
		if (debug)
		{

		//	display = output;

			// Out
				string str = "- Number of Objects: ";
				string val = to_string(circles.size());
				string out = str + val;
			//Out2
				str = "- Velocity: ";
				float height = 141;
				float px = Vel;
				float V = ((px/50) * (height/16));
				float r = ((radius / 50)* (height / 16));
				val = to_string(V);
				string out2 = str + val;
			//Out3
				str = "- Height: ";
				val = to_string(height);
				string out3 = str + val;
			//Out4
				str = "- Pixel Velocity: ";
				val = to_string(px);
				string out4 = str + val;
			//Out5
				str = "- Radious: ";
				val = to_string(r);
				string out5 = str + val;
			//Out6
				str = "- Area: ";
				val = to_string(((r)*(r)*(3.1415)));
				string out6 = str + val;


			//Screen
			cv::Point TP = Point(10, 25);
			putText(display, "Data", TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 255, 100));
			TP = Point(10, 50);
			putText(display, "- Velocity Conversion: ( V(PX) / 50(PX) )*( Y(CM) / 16(CM) ) = V(CM)", TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 255, 100));
			TP = Point(10, 75);
			putText(display, out, TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 255, 100));
			TP = Point(10, 100);
			putText(display, out2, TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 255, 100));
			TP = Point(10, 125);
			putText(display, out3, TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 255, 100));
			TP = Point(10, 150);
			putText(display, out4, TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 255, 100));
			TP = Point(10, 175);
			putText(display, "- Grid Check: ", TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 255, 100));
			TP = Point(10, 200);
			putText(display, out5, TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 255, 100));
			TP = Point(10, 225);
			putText(display, out6, TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 255, 100));

			// Point(x,y)
			int x = 445;
			int y = 303;
			TP = Point(x, y);
			putText(display, "+", TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 255, 100));
			TP = Point(x + 50, y + 50);
			putText(display, "+", TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 255, 100));

			// Point(x,y)
			x = 445;
			y = 303;
			TP = Point(x, y);
			putText(display, "+", TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 255, 100));
			TP = Point(x + 50, y + 50);
			putText(display, "+", TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 255, 100));


			// Point(x,y)
			x = 545;
			y = 403;
			TP = Point(x, y);
			putText(display, "+", TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 255, 100));
			TP = Point(x + 50, y + 50);
			putText(display, "+", TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 255, 100));
			
			if (0)
			{
				cv::Point PA = Point(cvRound(circles[0][0]), cvRound(circles[0][1]));
				cv::Point PB = Point(cvRound(circles[1][0]), cvRound(circles[1][1]));
				cv::line(display, PA, PB, cv::Scalar(255, 255, 255));
			}

			// Square detection:
			//End Square
		}
		//END TEST



		//Time start
		start = 0;
		start = std::clock();


		imshow("output", output);
		//imshow(windowName, display);

				std::cout << ">Final output returned in Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
				//End Time

	}
}

int main(int argc, char** argv)
{


	//Timing Test Behnam Saeedi


	// your test
	//std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;


	VideoCapture cap(0);
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 1024);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 768);
	if (!cap.isOpened()){
		return -1;
	}
	Mat src, src_gray;
	cap >> src;



	white = imread("C:\\Users\\TekBots\\Pictures\\white.png");

	//declare and initialize both parameters that are subjects to change
	int cannyThreshold = cannyThresholdInitialValue;
	int accumulatorThreshold = accumulatorThresholdInitialValue;

	// create the main window, and attach the trackbars
	namedWindow(windowName, WINDOW_AUTOSIZE);
	createTrackbar(cannyThresholdTrackbarName, windowName, &cannyThreshold, maxCannyThreshold);
	createTrackbar(accumulatorThresholdTrackbarName, windowName, &accumulatorThreshold, maxAccumulatorThreshold);

	namedWindow("output");
	// infinite loop to display
	// and refresh the content of the output image
	// until the user presses q or Q
	int key = 0;
	while (key != 'q' && key != 'Q')
	{
		//Time Start
		std::clock_t    start;
		start = std::clock();
		//



		cap >> src;
		// Convert it to gray
		cvtColor(src, src_gray, COLOR_BGR2GRAY);

		// Reduce the noise so we avoid false circle detection
		GaussianBlur(src_gray, src_gray, Size(9, 9), 2, 2);
		// those paramaters cannot be =0
		// so we must check here
		cannyThreshold = std::max(cannyThreshold, 1);
		accumulatorThreshold = std::max(accumulatorThreshold, 1);


		//Behnam Saeedi
		//distance between points test:
		bool debug2 = false;
		if (debug2)
		{
			Point2f a(10, 10);
			Point2f b(100, 100);

			double res = cv::norm(cv::Mat(a), cv::Mat(b));
			double res2 = cv::norm(a - b);
		//	std::cout << res << ", " << res2 << std::endl;

			int dist = 50;

			int width = src.size().width;
			int height = src.size().height;

			for (int i = 0; i<height; i += dist)
				cv::line(src, Point(0, i), Point(width, i), cv::Scalar(255, 255, 255));

			for (int i = 0; i<width; i += dist)
				cv::line(src, Point(i, 0), Point(i, height), cv::Scalar(255, 255, 255));

		}


		//runs the detection, and update the display
		HoughDetection(src_gray, src, cannyThreshold, accumulatorThreshold);

		// get user key
		key = waitKey(10);
		//Time check
		std::cout << ">Total Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
		std::cout << "__________________________________" << std::endl;
	}
	std::cout << ">Terminated..." << std::endl;
	return 0;
}