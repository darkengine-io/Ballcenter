#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "circles.h"
#include "debug.h"
#include "const.h"

using namespace cv;
using namespace std;
//Mat src_gray;

Mat src;
Mat out_src;

namespace bc {
	bool open(int id){
		cam = VideoCapture(id);
		cam.set(CV_CAP_PROP_FRAME_HEIGHT, IN_HEIGHT);
		cam.set(CV_CAP_PROP_FRAME_WIDTH, IN_WIDTH);
		if (!cam.isOpened()){
			return 0;
		}
		else{
			return 1;
		}
	}
	void get_frame(){
		cam >> src;
		src = src(Rect(225,197,266, 197));
		resize(src, src, Size(S_WIDTH, S_HEIGHT));
	}
	vector<Vec3f> find_circles(){
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
	void draw_circles(vector<Vec3f> circles){
		out_src = Mat::zeros(S_HEIGHT, S_WIDTH, CV_8UC3);
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
		frun(text_ovl(src, std::to_string((int)fps_end("loop")) + " fps", Point(0,15)));
		imshow(MAIN_WIN, src);
		imshow(OUT_WIN, out_src);
	}
}