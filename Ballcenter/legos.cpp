#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include "circles.h"
#include "debug.h"
#include "const.h"
#include "camera.h"
#include "legos.h"
#include <float.h>
#include <numeric>
#include <string>

using namespace cv;

namespace legos{
	int * Red;
	int * Green;
	int * Blue;
	int Position[3][10][2];
	int wait_for_key();
	vector<Point> find_smallest_square(const vector<vector<Point> >& squares);
	void findSquares(const Mat& image, vector<vector<Point> >& squares);
	void filter_squares(vector<vector<Point> >& squares);

	Mat src, out;

	void drawSquare(Mat& image, const vector<Point>& square, Scalar color){
		const Point* p = &square[0];
		int n = (int)square.size();
		polylines(image, &p, &n, 1, true, color, 3);
	}
	// 0 is blue, 1 is green, 2 is red
	int pick_dominant_color(Vec3b color){
		if (color[0] > color[1] && color[0] > color[2])
		{
			return 0; // blue
		}
		else if (color[1] > color[0] && color[1] > color[2])
		{
			return 1; // green
		}
		else if (color[2] > color[0] && color[2] > color[1])
		{
			return 2; // red
		}
	}

	// the function draws all the squares in the image
	void drawSquares(Mat& image, Mat& out, const vector<vector<Point> >& squares)
	{
		for (size_t i = 0; i < squares.size(); i++)
		{
			Point zero(0.0f, 0.0f);
			Point sum = std::accumulate(squares[i].begin(), squares[i].end(), zero);
			Point mean_point(sum.x / squares[i].size(), sum.y / squares[i].size());
			Vec3b color = image.at<Vec3b>(mean_point);
			drawSquare(out, squares[i], Scalar(color));
			text_ovl(out, std::to_string(pick_dominant_color(color)), mean_point, Scalar(color));
			switch (pick_dominant_color(color))
			{
			case(0) :
				*Blue = *Blue + 1;
				Position[3][(*Blue) - 1][0] = mean_point.x;
				Position[3][(*Blue) - 1][1] = mean_point.y;
				break;
			case(1) :
				*Green = *Green + 1;
				Position[3][(*Green) - 1][0] = mean_point.x;
				Position[3][(*Green) - 1][1] = mean_point.y;
				break;
			case(2) :
				*Red = *Red + 1;
				Position[3][(*Red) - 1][0] = mean_point.x;
				Position[3][(*Red) - 1][1] = mean_point.y;
				break;
			}
		}
	}
	//vector<vector<Point>> squares;

	square pass(Mat& image, int Count)
	{
		Point zero(0.0f, 0.0f);
		Point sum = std::accumulate(squares_g[Count].begin(), squares_g[Count].end(), zero);
		Point mean_point(sum.x / squares_g[Count].size(), sum.y / squares_g[Count].size());
		Vec3b color = image.at<Vec3b>(mean_point);
		square s;
		s.center = mean_point;
		s.center.x /= (float)image.cols / (float)MAP_WIDTH;
		s.center.y /= (float)image.rows / (float)MAP_HEIGHT;
		s.color = pick_dominant_color(color);
		return s;
	}
	void legos(Camera& cam, Mat* Map, int * red, int * green, int * blue){
		Red = red;
		Green = green;
		//vector<vector<Point>> squares;
		Blue = blue;
		namedWindow(LEGO_WIN, CV_WINDOW_NORMAL);
		cvSetWindowProperty(LEGO_WIN, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
		while (true){
			out = Mat::zeros(cam.aoi.height, cam.aoi.width, CV_8UC3);

			out = Scalar(LEGO_COLOR);
			imshow(LEGO_WIN, *Map);
			wait_for_key();
			imshow(LEGO_WIN, out);
			waitKey(500);

			cam.get_frame();
			cam.get_frame();
			vector<vector<Point>> squares;
			findSquares(cam.src, squares);
			filter_squares(squares);
			cam.src.copyTo(out);
			drawSquares(cam.src, out, squares);
			resize(out, out, Size(OUT_WIDTH, OUT_HEIGHT));
			imshow(LEGO_WIN, out);
			if (wait_for_key() == 106){
				continue;
			}
			else {
				squares_g = squares;
				break;
			}
		}

		// TODO set up towers 
		destroyWindow(LEGO_WIN);
	}

	void filter_squares(vector<vector<Point> >& squares){
		bool finished;
		while (true){
			finished = true;
			for (size_t i = 0; i < squares.size(); i++){
				bool stop = false;
				for (size_t j = 0; j < squares.size(); j++){
					if (i >= squares.size() || j >= squares.size()){
						stop = true;
						break;
					}
					if (i != j && ((abs(squares[i][0].x - squares[j][0].x) < 10 && abs(squares[i][0].y - squares[j][0].y) < 10) ||
						(abs(squares[i][2].x - squares[j][2].x) < 10 && abs(squares[i][2].y - squares[j][2].y) < 10))){
						squares.erase(squares.begin() + j);
						finished = false;
					}
				}
				if (stop){
					finished = false;
					break;
				}
			}
			if (finished)
				break;
		}
		while (true){
			finished = true;
			for (size_t i = 0; i < squares.size(); i++){
				for (int j = 0; j < squares[i].size(); j++){
					if (squares[i][j].x < 20 && squares[i][j].y < 20){
						squares.erase(squares.begin() + i);
						finished = false;
						break;
					}
				}
			}
			if (finished)
				break;
		}
	}

	int wait_for_key(){
		int key;
		while (true){
			key = waitKey(10);
			if (key > 0)
				break;
		}
		return key;
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
			for (int l = 0; l < LEGO_ITER; l++)
			{
				// hack: use Canny instead of zero threshold level.
				// Canny helps to catch squares with gradient shading
				if (l == 0)
				{
					// apply Canny. Take the upper threshold from slider
					// and set the lower to 0 (which forces edges merging)
					Canny(gray0, gray, 0, LEGO_SQ_THRESH, 5);
					// dilate canny output to remove potential
					// holes between edge segments
					dilate(gray, gray, Mat(), Point(-1, -1));
				}
				else
				{
					// apply threshold if l!=0:
					//     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
					gray = gray0 >= (l + 1) * 255 / LEGO_ITER;
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