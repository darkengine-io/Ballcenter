#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "circles.h"
#include "debug.h"
#include "const.h"

using namespace cv;
using namespace std;
//Mat src_gray;

//Behnam Saeedi TEST DELETE ME
float radius;
//END TEST
Mat out_src;

namespace bc {
	cv::vector<cv::Vec<float, 3>>* find_circles(Mat& src){
		Mat input_gray;
		/// Convert it to gray
		cvtColor(src, input_gray, CV_BGR2GRAY);

		/// Apply the Hough Transform to find the circles
		ts("circle");
		HoughCircles(input_gray, circles, CV_HOUGH_GRADIENT, 1, MIN_DIST, UPPER_EDGE, CENTER_THRESH, MIN_RADIUS, MAX_RADIUS);
		te("circle");
		return &circles;
	}
	void draw_circles(Mat& src, float x_scale, float y_scale){
		out_src = Mat::zeros(OUT_HEIGHT, OUT_WIDTH, CV_8UC3);

		out_src = Scalar(OUT_BG);
		for (size_t i = 0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			/*int*/ radius = cvRound(circles[i][2]) + 3;
			// circle center
			circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			// circle outline
			circle(src, center, radius, Scalar(0, 0, 255), 3, 8, 0);

			Point scaled_center = Point((float)center.x * x_scale, (float)center.y * y_scale);
			circle(out_src, scaled_center, radius * x_scale, Scalar(CIRCLE_COLOR), 3, 8, 0);
		}
		frun(text_ovl(src, std::to_string((int)fps_end("loop")) + " fps", Point(0,15), Scalar(255, 0, 100)));


		//TEST CODE : Behnam Saeedi(Ben) ***********DELETE ME LATER ... NOT JUST YET!
		bool Temp_debug = true;
		if (Temp_debug)
		{

			//	display = output;

			// Out
			string str = "- Number of Objects: ";
			string val = to_string(circles.size());
			string out = str + val;
			//Out2
			str = "- Velocity: ";

			float Vel = 0;
			Mat display = src;
			//Mat display = out_src;

			float height = 141;
			float px = Vel;
			float V = ((px / 50) * (height / 16));
			float r = (((radius - 3) / 50)* (height / 16));
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
			str = "- Radius: ";
			val = to_string(r);
			string out5 = str + val;
			//Out6
			str = "- Area: ";
			val = to_string(4*((r)*(r)*(3.1415)));
			string out6 = str + val;
			//Out7
			str = "- Volume: ";
			val = to_string((4*(r)*(r)*(r)*(3.1415))/3);
			string out7 = str + val;

			//Screen
			cv::Point TP = Point(10, 25);
			putText(display, "Data", TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 100, 0));
			TP = Point(10, 50);
			putText(display, "- ( V(PX) / 50(PX) )*( Y(CM) / 16(CM) ) = V(CM)", TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 100, 0));
			TP = Point(10, 75);
			putText(display, out, TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 100, 0));
			TP = Point(10, 100);
			putText(display, out2, TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 100, 0));
			TP = Point(10, 125);
			putText(display, out3, TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 100, 0));
			TP = Point(10, 150);
			putText(display, out4, TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 100, 0));
			TP = Point(10, 175);
			putText(display, "- Grid Check: ", TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 100, 0));
			TP = Point(10, 200);
			putText(display, out5, TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 100, 0));
			TP = Point(10, 225);
			putText(display, out6, TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 100, 0));
			TP = Point(10, 250);
			putText(display, out7, TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 100, 0));

			// Point(x,y)
			int x = 445;
			int y = 303;
			TP = Point(x, y);
			putText(display, "+", TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 100, 0));
			TP = Point(x + 50, y + 50);
			putText(display, "+", TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 100, 0));

			// Point(x,y)
			x = 445;
			y = 303;
			TP = Point(x, y);
			putText(display, "+", TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 100, 0));
			TP = Point(x + 50, y + 50);
			putText(display, "+", TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 100, 0));


			// Point(x,y)
			x = 545;
			y = 403;
			TP = Point(x, y);
			putText(display, "+", TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 100, 0));
			TP = Point(x + 50, y + 50);
			putText(display, "+", TP, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 100, 0));

			// Square detection:
			/*
			vector<Point> approx;

			static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
			{
				double dx1 = pt1.x - pt0.x;
				double dy1 = pt1.y - pt0.y;
				double dx2 = pt2.x - pt0.x;
				double dy2 = pt2.y - pt0.y;
				return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
			}

			void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
			{
				int fontface = cv::FONT_HERSHEY_SIMPLEX;
				double scale = 0.4;
				int thickness = 1;
				int baseline = 0;

				cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
				cv::Rect r = cv::boundingRect(contour);

				cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
				cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255, 255, 255), CV_FILLED);
				cv::putText(im, label, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
			}
			// Find contours
			std::vector<std::vector<cv::Point>> contours;
			cv::findContours(display.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

			std::vector<cv::Point> approx;
			cv::Mat dst = src.clone();

			for (int i = 0; i < contours.size(); i++)
			{
				// Approximate contour with accuracy proportional
				// to the contour perimeter
				cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

				// Skip small or non-convex objects 
				if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
					continue;

				if (approx.size() == 3)
				{
					   // Triangles
				}
				else if (approx.size() >= 4 && approx.size() <= 6)
				{
					// Number of vertices of polygonal curve
					int vtc = approx.size();

					// Get the cosines of all corners
					std::vector<double> cos;
					for (int j = 2; j < vtc + 1; j++)
						cos.push_back(angle(approx[j%vtc], approx[j - 2], approx[j - 1]));

					// Sort ascending the cosine values
					std::sort(cos.begin(), cos.end());

					// Get the lowest and the highest cosine
					double mincos = cos.front();
					double maxcos = cos.back();

					// Use the degrees obtained above and the number of vertices
					// to determine the shape of the contour
					if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3)
						//Rectangle
					else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27)
						//Pentagon
					else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45)
						//Hexagon
				}
				else
				{
					// Detect and label circles
					double area = cv::contourArea(contours[i]);
					cv::Rect r = cv::boundingRect(contours[i]);
					int radius = r.width / 2;

					if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
						std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
						//Circle
				}
			}
}

*/
			//End Square
		}
		//END TEST


		imshow(MAIN_WIN, src);
		imshow(OUT_WIN, out_src);
	}
}