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
	vector<Vec3f> find_circles(Mat src){
		Mat input_gray;
		/// Convert it to gray
		cvtColor(src, input_gray, CV_BGR2GRAY);

		/// Reduce the noise so we avoid false circle detection
		GaussianBlur(input_gray, input_gray, Size(9, 9), 2, 2);

		vector<Vec3f> circles;

		/// Apply the Hough Transform to find the circles
		ts("circle");
		HoughCircles(input_gray, circles, CV_HOUGH_GRADIENT, 1, MIN_DIST, UPPER_EDGE, CENTER_THRESH, MIN_RADIUS, MAX_RADIUS);
		te("circle");
		return circles;
	}
	void draw_circles(vector<Vec3f> circles, Mat src){
		out_src = Mat::zeros(src.rows, src.cols, CV_8UC3);
		out_src = Scalar(OUT_BG);
		for (size_t i = 0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// circle center
			circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			// circle outline
			circle(src, center, radius, Scalar(0, 0, 255), 3, 8, 0);

			circle(out_src, center, radius, Scalar(CIRCLE_COLOR), 3, 8, 0);
		}
#if S_HEIGHT !=  OUT_HEIGHT || S_WIDTH != OUT_WIDTH
		resize(out_src, out_src, Size(OUT_WIDTH, OUT_HEIGHT));
#endif
		frun(text_ovl(src, std::to_string((int)fps_end("loop")) + " fps", Point(0,15), Scalar(255, 0, 100)));
		imshow(MAIN_WIN, src);
		imshow(OUT_WIN, out_src);
	}
}