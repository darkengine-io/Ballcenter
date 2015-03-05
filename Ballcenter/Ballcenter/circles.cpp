#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "circles.h"
#include "debug.h"
#include "const.h"

using namespace cv;
using namespace std;
//Mat src_gray;

Mat out_src;
namespace bc {
	void find_circles(Mat& src, cv::vector<cv::Vec<float, 3>>& circles){
		Mat input_gray(src.rows, src.cols, CV_8UC3);
		int from_to[] = { 0,0, 1,1, 1,2 };
		mixChannels(&src, 1, &input_gray, 1, from_to, 3); // drop color channels
		cvtColor(input_gray, input_gray, CV_BGR2GRAY);

		/// Apply the Hough Transform to find the circles
		ts("circle");
		HoughCircles(input_gray, circles, CV_HOUGH_GRADIENT, 1, MIN_DIST, UPPER_EDGE, CENTER_THRESH, MIN_RADIUS, MAX_RADIUS);
		te("circle");
		imshow(CAM_WIN, input_gray);
	}
	void draw_circles(Mat& src, vector<Vec3f>& circles){
		out_src = Mat::zeros(OUT_HEIGHT, OUT_WIDTH, CV_8UC3);

		out_src = Scalar(OUT_BG);
		for (size_t i = 0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// circle center
			circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			// circle outline
			circle(src, center, radius, Scalar(0, 0, 255), 3, 8, 0);

			Point scaled_center = Point((float)center.x, (float)center.y);
			circle(out_src, scaled_center, radius, Scalar(CIRCLE_COLOR), 3, 8, 0);
		}
		frun(text_ovl(src, std::to_string((int)fps_end("loop")) + " fps", Point(0,15), Scalar(255, 0, 100)));
	}
	void draw_blobs(Mat& src){
		Mat canny_output;
		vector<vector<Point>> blobs;
		vector<Vec4i> hierarchy;
		Canny(src, canny_output, BLOB_THRESH, BLOB_THRESH * 2, 3);
		/// Find contours
		findContours(canny_output, blobs, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		/// Draw contours
		//for (int i = 0; i< blobs.size(); i++)
		//{
			drawContours(out_src, blobs, -1, Scalar(100,0,155), CV_FILLED, 8, hierarchy, 0, Point());
		//}
	}

	void redraw(Mat& src){
		resize(out_src, out_src, Size(OUT_WIDTH, OUT_HEIGHT));
		imshow(MAIN_WIN, src);
		imshow(OUT_WIN, out_src);
	}
}