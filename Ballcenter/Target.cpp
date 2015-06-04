#include "Target.h"


using namespace cv;

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
	( std::ostringstream() << std::dec << x ) ).str()

char Target::path(int *x, int *y, int t, char type, char Dir, int *alive, int inity, int initx, int *wave, int *gp, int * speed, int *targetHit, int *HD, Mat * Out, getLego* start, Tower_C * TowerC, Tower_T * TowerT, Tower_S * TowerS)
{
	if (*x < initx + 65 && (*y >= inity - *speed && *y <= inity + *speed) && *alive)
	{
		*x = *x + *speed;
		Dir = 'R';
	}
	else
	{
		if (*y > inity - (240 - 110) && ((*x >= initx + 65 - *speed) && (*x <= initx + 65 + *speed)) && *alive)
		{
			*y = *y - *speed;
			Dir = 'U';
		}
		else
		{
			if (*x < initx + 185 && ((*y <= inity - (240 - 110) + *speed)&&(*y >= inity - (240 - 110) - *speed)) && *alive)
			{
				*x = *x + *speed;
				Dir = 'R';
			}
			else
			{
				if (*y < inity - (217 - 265) && ((*x >= initx - *speed + 185) && (*x <= initx + *speed + 185)) && *alive)
				{
					*y = *y + *speed;
					Dir = 'D';
				}
				else
				{
					if (*x < initx + 345 && ((*y >= inity - *speed - (217 - 265)) && (*y <= inity + *speed - (217 - 265))) && *alive)
					{
						*x = *x + *speed;
						Dir = 'R';
					}
					else
					{
						if (*y > inity - (230 - 190) && ((*x >= initx - *speed + 345)&&(*x <= initx + *speed + 345)) && *alive)
						{
							*y = *y - *speed;
							Dir = 'U';
						}
						else
						{
							if (*x < initx + 535 && ((*y >= inity - *speed - (230 - 190))&&(*y <= inity - (230 - 190) + *speed)) && *alive)
							{
								*x = *x + *speed;
								Dir = 'R';
							}
							else
							{
								if (((*x >= initx - *speed)&&(*x <= initx + *speed)) + 535 && *alive)
								{
									*gp = *gp - 1;
								//	waitKey();
								}
								if (*gp < 1)
								{
									*alive = 0;
									putText(*Out, "Game Over at wave: " + SSTR(*wave), Point(300, 400), cv::FONT_HERSHEY_DUPLEX, 1, Scalar::all(255), 2, 3);
									waitKey(500);
									return 'E';
								}

								*wave = *wave + 1;
								
								*x = initx;
								*y = inity;
								*alive = 1;
								*targetHit = 0;
								*HD = 0;
								//start->scan(TowerC, TowerT, TowerS);
							}
						}
					}
				}
			}
		}
	}

	return Dir;
}

Target::Target(int wave, int HP)
{
	speed = wave;
	health = HP;
	Location[0] = 0;
	Location[1] = 0;
}
void Target::Draw(Mat Screen, int x, int y, char Dir)
{
	int step;
	if ((((x + y) / 3) % 2) == 1)
		step = 2;
	else
		step = 1;
	Location[0] = x + 10;
	Location[1] = y - 10;
	Mat tmp;
	string name = "im/T";
	name = name + Dir + "-" + SSTR(step) + ".png";
	tmp = imread(name, CV_LOAD_IMAGE_UNCHANGED);
	Mat tank;
	resize(tmp, tank, Size(25, 25));
	cv::Rect roi(Point(Location[0], Location[1]), tank.size());
	tank.copyTo(Screen(roi));

	Point barFrameA(0, -29);
	Point barFrameB(40, -21);
	if (health < 1)
		health = 0;
	if (health < 10)
		rectangle(Screen, Point(x, y) + Point(0, -25), Point(x, y) + Point(40 - health, -20), Scalar(0, 255, 0), 2, 2.0);
	else{
		if (health < 30)
			rectangle(Screen, Point(x, y) + Point(0, -25), Point(x, y) + Point(40 - health, -20), Scalar(0, 255, 255), 2, 2.0);
		else{
			rectangle(Screen, Point(x, y) + Point(0, -25), Point(x, y) + Point(40 - health, -20), Scalar(0, 0, 255), 2, 2.0);
		}
	}
	rectangle(Screen, Point(x, y) + barFrameA, Point(x, y) + barFrameB, Scalar(225, 0, 0), 3, 1.0);

}
void Target::Hit(Mat Screen, int x, int y, int num, char Dir)
{
	Location[0] = x + 10;
	Location[1] = y - 10;
	string name = "im/F";
	name = name + Dir;
	name = name + "-";
	name = name + SSTR(num);
	name = name + ".png";
	Mat tmp;
	if (Dir == 'R')
		tmp = imread(name, CV_LOAD_IMAGE_UNCHANGED);
	else
		if (Dir == 'L')
			tmp = imread(name, CV_LOAD_IMAGE_UNCHANGED);
		else
			if (Dir == 'U')
				tmp = imread(name, CV_LOAD_IMAGE_UNCHANGED);
			else
				tmp = imread(name, CV_LOAD_IMAGE_UNCHANGED);

	Mat tank;
	resize(tmp, tank, Size(25, 25));
	cv::Rect roi(Point(Location[0], Location[1]), tank.size());
	tank.copyTo(Screen(roi));

	Point barFrameA(0, -29);
	Point barFrameB(40, -21);
	if (health < 1)
		health = 0;
	if (health < 10)
		rectangle(Screen, Point(x, y) + Point(0, -25), Point(x, y) + Point(40 - health, -20), Scalar(0, 255, 0), 2, 2.0);
	else{
		if (health < 30)
			rectangle(Screen, Point(x, y) + Point(0, -25), Point(x, y) + Point(40 - health, -20), Scalar(0, 255, 255), 2, 2.0);
		else{
			rectangle(Screen, Point(x, y) + Point(0, -25), Point(x, y) + Point(40 - health, -20), Scalar(0, 0, 255), 2, 2.0);
		}
	}
	rectangle(Screen, Point(x, y) + barFrameA, Point(x, y) + barFrameB, Scalar(225, 0, 0), 3, 1.0);

}
