#ifndef SSTR( x )
#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
#endif

#include"Tower.h"
#include "const.h"
#include "debug.h"
#include "circles.h"
#include "camera.h"
#include "calibrate.h"
#include "legos.h"
#include "data.h"
#include "Print.h"

using namespace cv;

cv::vector<cv::Vec<float, 3>> circles;
vector<vector<Point>> blobs;
vector<Vec4i> hierarchy;
Camera cam;

getLego::getLego(int * TR, int * TG, int * TB, Mat* Map)
{
	Red = 0;
	Green = 0;
	Blue = 0;

	dp("Starting");

	dp("Setting up capture");
	cam.open(0);

	dp("Calibrating");
	cal::calibrate(cam);

	dp("Legos");
	legos::legos(cam, Map, &Red, &Green, &Blue);
	int total = Red + Green + Blue;
	if (1)
		*TR = Red;
	*TG = Green;
	*TB = Blue;
}
void getLego::scan(Tower_C * TowerC, Tower_T * TowerT, Tower_S * TowerS){
	legos::rescan(cam, &Red, &Green, &Blue);
	getLego::set(TowerC, TowerT, TowerS);
}
int getLego::get(int color, int number, int axis)
{
	return position[color][number][axis];
}
void getLego::set(Tower_C * TowerC, Tower_T * TowerT, Tower_S * TowerS)
{
	int bc = 0, gc = 0, rc = 0;
	for (int i = 0; i < (Red + Green + Blue); i++){
		square s = legos::pass(cam.src, i);
		switch (s.color){
		case 0:
			TowerS[bc].Location[0] = s.center.x;
			TowerS[bc].Location[1] = s.center.y;
			bc++;
			break;
		case 1:
			TowerT[gc].Location[0] = s.center.x;
			TowerT[gc].Location[1] = s.center.y;
			gc++;
			break;
		case 2:
			TowerC[rc].Location[0] = s.center.x;
			TowerC[rc].Location[1] = s.center.y;
			rc++;
			break;
		}
	}
}



Tower_C::Tower_C(int x, int y)
{
	Damage = 1;
	Range = 90;
	Rate = 5;
	Location[0] = x;
	Location[1] = y;
	Color[0] = 0;
	Color[1] = 0;
	Color[2] = 225;
	Ammo = 150;
}
Tower_C::Tower_C()
{
	Damage = 1;
	Range = 90;
	Rate = 5;
	Color[0] = 0;
	Color[1] = 0;
	Color[2] = 225;
	Ammo = 150;
}
Tower_T::Tower_T(int x, int y)
{
	Damage = 2;
	Range = 80;
	Rate = 4;
	Location[0] = x;
	Location[1] = y;
	Color[0] = 0;
	Color[1] = 225;
	Color[2] = 0;
	Ammo = 100;
}
Tower_T::Tower_T()
{
	Damage = 2;
	Range = 80;
	Rate = 4;
	Color[0] = 0;
	Color[1] = 225;
	Color[2] = 0;
	Ammo = 100;

}
Tower_S::Tower_S(int x, int y)
{
	Damage = 3;
	Range = 70;
	Rate = 3;
	Location[0] = x;
	Location[1] = y;
	Color[0] = 225;
	Color[1] = 0;
	Color[2] = 0;
	Ammo = 50;
}
Tower_S::Tower_S()
{
	Damage = 3;
	Range = 70;
	Rate = 3;
	Color[0] = 225;
	Color[1] = 179;
	Color[2] = 72;
	Ammo = 50;
}

void Tower_C::Draw(Mat &Screen)
{
	for (int i = 0; i < Ammo; i++)
	{
		Point * position = ammoCircle(i);
		line(Screen, position[0], position[1], Scalar(Color[0], Color[1], Color[2]), 1, 1, 0);
	}
	for (int i = Ammo; i < 150; i++)
	{
		Point * position = ammoCircle(i);
		line(Screen, position[0], position[1], Scalar(0, 0, 0), 1, 1, 0);
	}
}
void Tower_T::Draw(Mat &Screen)
{
	for (int i = 0; i < Ammo; i++)
	{
		Point * position = ammoCircle(i);
		line(Screen, position[0], position[1], Scalar(Color[0], Color[1], Color[2]), 1, 1, 0);
	}
	for (int i = Ammo; i < 100; i++)
	{
		Point * position = ammoCircle(i);
		line(Screen, position[0], position[1], Scalar(0, 0, 0), 1, 1, 0);
	}

}
void Tower_S::Draw(Mat &Screen)
{ 
	for (int i = 0; i < Ammo; i++)
	{
		Point * position = ammoCircle(i);
		line(Screen, position[0], position[1], Scalar(Color[0], Color[1], Color[2]), 1, 1, 0);
	}
	for (int i = Ammo; i < 50; i++)
	{
		Point * position = ammoCircle(i);
		line(Screen, position[0], position[1], Scalar(0, 0, 0), 1, 1, 0);
	}
}

bool Tower_C::Reload(uint8_t * Data)
{
	return ((Ammo < 150) && (int(Data[2]) > 100));
}
bool Tower_T::Reload(uint8_t * Data)
{
	return ((Ammo < 100) && (int(Data[2]) > 100));
}
bool Tower_S::Reload(uint8_t * Data)
{
	return ((Ammo < 50) && (int(Data[2]) > 100));
}
Point* Tower_C::ammoCircle(int i)
{
	float angle = (2 * PI) / 150;
	cv::Point * temp = new cv::Point[2];
	temp[0].x = Range * cos(i * angle) + Location[0];
	temp[0].y = Range * sin(i * angle) + Location[1];
	temp[1].x = (Range + 2) * cos(i * angle) + Location[0];
	temp[1].y = (Range + 2) * sin(i * angle) + Location[1];

	return temp;
}
Point* Tower_T::ammoCircle(int i)
{
	float angle = (2 * PI) / 100;
	cv::Point * temp = new cv::Point[2];
	temp[0].x = Range * cos(i * angle) + Location[0];
	temp[0].y = Range * sin(i * angle) + Location[1];
	temp[1].x = (Range + 2) * cos(i * angle) + Location[0];
	temp[1].y = (Range + 2) * sin(i * angle) + Location[1];

	return temp;
}
Point* Tower_S::ammoCircle(int i)
{
	float angle = (2 * PI) / 50;
	cv::Point * temp = new cv::Point[2];
	temp[0].x = Range * cos(i * angle) + Location[0];
	temp[0].y = Range * sin(i * angle) + Location[1];
	temp[1].x = (Range + 2) * cos(i * angle) + Location[0];
	temp[1].y = (Range + 2) * sin(i * angle) + Location[1];

	return temp;
}