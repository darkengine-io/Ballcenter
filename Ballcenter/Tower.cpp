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
	int bc = 0, gc = 0, rc = 0;
	for (int i = 0; i < (Red + Green + Blue);i++){
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

