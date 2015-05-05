#include"Tower.h"
#include "const.h"
#include "debug.h"
#include "circles.h"
#include "camera.h"
#include "calibrate.h"
#include "legos.h"
#include "data.h"

using namespace cv;

cv::vector<cv::Vec<float, 3>> circles;
vector<vector<Point>> blobs;
vector<Vec4i> hierarchy;
Camera cam;

getLego::getLego(int * TR,int * TG,int * TB)
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
	legos::legos(cam,&Red,&Green,&Blue);
	int total = Red + Green + Blue;
	if (1)
	*TR = Red;
	*TG = Green;
	*TB = Blue;
}
int getLego::get(int color, int number, int axis)
{
	return position[color][number][axis];
}
void getLego::set(Tower_C * TowerC ,Tower_T * TowerT ,Tower_S * TowerS)
{
	
	for(int i = 0; i < Red; i++)
	{
		TowerC[i].Location[0] = legos::pass(2, i).x;
		TowerC[i].Location[1] = legos::pass(2, i).y;
	}
	for(int i = 0; i < Green; i++)
	{
		TowerT[i].Location[0] = legos::pass(2, i).x;
		TowerT[i].Location[1] = legos::pass(2, i).y;
	}
	for (int i = 0; i < Blue; i++)
	{
		TowerS[i].Location[0] = legos::pass(2, i).x;
		TowerS[i].Location[1] = legos::pass(2, i).y;
	}

}



Tower_C::Tower_C (int x,int y)
{
	Damage = 1;
	Range = 70;
	Capacity = 5;
	Location[0] = x;
	Location[1] = y;
	Color[0] = 0;
	Color[1] = 0;
	Color[2] = 225;
}
Tower_C::Tower_C()
{
	Damage = 1;
	Range = 70;
	Capacity = 5;
	Color[0] = 0;
	Color[1] = 0;
	Color[2] = 225;
}
Tower_T::Tower_T (int x,int y)
{
	Damage = 2;
	Range = 60;
	Capacity = 4;
	Location[0] = x;
	Location[1] = y;
	Color[0] = 0;
	Color[1] = 225;
	Color[2] = 0;
}
Tower_T::Tower_T()
{
	Damage = 2;
	Range = 60;
	Capacity = 4;
	Color[0] = 0;
	Color[1] = 225;
	Color[2] = 0;

}
Tower_S::Tower_S (int x,int y)
{
	Damage = 3;
	Range = 50;
	Capacity = 3;
	Location[0] = x;
	Location[1] = y;
	Color[0] = 225;
	Color[1] = 0;
	Color[2] = 0;
}
Tower_S::Tower_S()
{
	Damage = 3;
	Range = 50;
	Capacity = 3;
	Color[0] = 225;
	Color[1] = 0;
	Color[2] = 0;
}

void Tower_C::Draw(Mat &Screen)
{
	circle(Screen,Point(Location[0],Location[1]),Range,Scalar(Color[0],Color[1],Color[2]),Damage,1,0);
}
void Tower_T::Draw(Mat &Screen)
{
	circle(Screen,Point(Location[0],Location[1]),Range,Scalar(Color[0],Color[1],Color[2]),Damage,1,0);
}
void Tower_S::Draw(Mat &Screen)
{
	circle(Screen,Point(Location[0],Location[1]),Range,Scalar(Color[0],Color[1],Color[2]),Damage,1,0);
}

