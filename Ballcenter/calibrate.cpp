#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "circles.h"
#include "debug.h"
#include "const.h"
#include "camera.h"
#include "calibrate.h"
#include <float.h>

using namespace cv;

namespace cal{
	void wait_for_key();
	vector<Point> find_smallest_square(const vector<vector<Point> >& squares);
	void findSquares(const Mat& image, vector<vector<Point> >& squares);

	Mat src, out;

	void drawSquare(Mat& image, const vector<Point>& square, Scalar color){
		const Point* p = &square[0];
		int n = (int)square.size();
		polylines(image, &p, &n, 1, true, color, 3);
	}

	// the function draws all the squares in the image
	void drawSquares(Mat& image, const vector<vector<Point> >& squares, Scalar color)
	{
		for (size_t i = 0; i < squares.size(); i++)
		{
			drawSquare(image, squares[i], color);
		}
	}

	void calibrate(Camera& cam){
		cam.cam.set(CV_CAP_PROP_FRAME_WIDTH, CAL_WIDTH);
		cam.cam.set(CV_CAP_PROP_FRAME_HEIGHT, CAL_HEIGHT);
		namedWindow(CAL_WIN, CV_WINDOW_AUTOSIZE);
		out = Mat::zeros(OUT_HEIGHT, OUT_WIDTH, CV_8UC3);

		out = Scalar(CAL_COLOR);
		imshow(CAL_WIN, out);
		wait_for_key();

		cam.get_raw_frame();
		cam.get_raw_frame();
		vector<vector<Point>> squares;
		findSquares(cam.src, squares);
		drawSquares(cam.src, squares, Scalar(0, 255, 0));
		vector<Point> smallest_square = find_smallest_square(squares);
		drawSquare(cam.src, smallest_square, Scalar(255, 0, 0));
		resize(cam.src, cam.src, Size(OUT_WIDTH, OUT_HEIGHT));
		imshow(CAL_WIN, cam.src);
		wait_for_key();
		
		cam.aoi = boundingRect(Mat(smallest_square) * ((float)IN_HEIGHT / (float)CAL_HEIGHT));
		cam.y_scale = (float)OUT_HEIGHT / cam.aoi.height;
		cam.x_scale = (float)OUT_WIDTH / cam.aoi.width;
		destroyWindow(CAL_WIN);
		cam.cam.set(CV_CAP_PROP_FRAME_WIDTH, IN_WIDTH);
		cam.cam.set(CV_CAP_PROP_FRAME_HEIGHT, IN_HEIGHT);
	}

	void wait_for_key(){
		while (waitKey(10) < 0);
	}

	vector<Point> find_smallest_square(const vector<vector<Point> >& squares)
	{
		vector<Point> biggest_square;

		float min_area = FLT_MAX;
		int max_square_idx = 0;
		const int n_points = 4;

		for (size_t i = 0; i < squares.size(); i++)
		{
			// Convert a set of 4 unordered Points into a meaningful cv::Rect structure.
			Rect rectangle = boundingRect(Mat(squares[i]));

			//        cout << "find_largest_square: #" << i << " rectangle x:" << rectangle.x << " y:" << rectangle.y << " " << rectangle.width << "x" << rectangle.height << endl;

			// Store the index position of the biggest square found
			if (rectangle.area() < min_area)
			{
				min_area = (float)rectangle.area();
				max_square_idx = i;
			}
		}

		biggest_square = squares[max_square_idx];
		return biggest_square;
	}

	// helper function:
	// finds a cosine of angle between vectors
	// from pt0->pt1 and from pt0->pt2
	static double angle(Point pt1, Point pt2, Point pt0)
	{
		double dx1 = pt1.x - pt0.x;
		double dy1 = pt1.y - pt0.y;
		double dx2 = pt2.x - pt0.x;
		double dy2 = pt2.y - pt0.y;
		return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
	}

	// returns sequence of squares detected on the image.
	// the sequence is stored in the specified memory storage
	static void findSquares(const Mat& image, vector<vector<Point> >& squares)
	{
		squares.clear();

		Mat pyr, timg, gray0(image.size(), CV_8U), gray;

		// down-scale and upscale the image to filter out the noise
		pyrDown(image, pyr, Size(image.cols / 2, image.rows / 2));
		pyrUp(pyr, timg, image.size());
		vector<vector<Point> > contours;

		// find squares in every color plane of the image
		for (int c = 0; c < 3; c++)
		{
			int ch[] = { c, 0 };
			mixChannels(&timg, 1, &gray0, 1, ch, 1);

			// try several threshold levels
			for (int l = 0; l < CAL_ITER; l++)
			{
				// hack: use Canny instead of zero threshold level.
				// Canny helps to catch squares with gradient shading
				if (l == 0)
				{
					// apply Canny. Take the upper threshold from slider
					// and set the lower to 0 (which forces edges merging)
					Canny(gray0, gray, 0, SQ_THRESH, 5);
					// dilate canny output to remove potential
					// holes between edge segments
					dilate(gray, gray, Mat(), Point(-1, -1));
				}
				else
				{
					// apply threshold if l!=0:
					//     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
					gray = gray0 >= (l + 1) * 255 / CAL_ITER;
				}

				// find contours and store them all as a list
				findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

				vector<Point> approx;

				// test each contour
				for (size_t i = 0; i < contours.size(); i++)
				{
					// approximate contour with accuracy proportional
					// to the contour perimeter
					approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

					// square contours should have 4 vertices after approximation
					// relatively large area (to filter out noisy contours)
					// and be convex.
					// Note: absolute value of an area is used because
					// area may be positive or negative - in accordance with the
					// contour orientation
					if (approx.size() == 4 &&
						fabs(contourArea(Mat(approx))) > 1000 &&
						isContourConvex(Mat(approx)))
					{
						double maxCosine = 0;

						for (int j = 2; j < 5; j++)
						{
							// find the maximum cosine of the angle between joint edges
							double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
							maxCosine = MAX(maxCosine, cosine);
						}

						// if cosines of all angles are small
						// (all angles are ~90 degree) then write quandrange
						// vertices to resultant sequence
						if (maxCosine < 0.3)
							squares.push_back(approx);
					}
				}
			}
		}
	}
}