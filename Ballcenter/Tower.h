#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include "Serial.h"
#include "camera.h"
using namespace cv;

class Tower_C
{
public:
	int Damage;
	int Range;
	int Capacity;
	int Location[2];
	int Color[3];
	Tower_C(int x = 0, int y = 0);
	void Draw(Mat &Screen);
	bool Reload(Serial * Data);
	int Ammo;
	//	private:
};
class Tower_T
{
public:
	int Damage;
	int Range;
	int Capacity;
	int Location[2];
	int Color[3];
	Tower_T(int x=0, int y=0);
	void Draw(Mat &Screen);
	bool Reload(Serial * Data);
	int Ammo;
	//	private:
};
class Tower_S
{
public:
	int Damage;
	int Range;
	int Capacity;
	int Location[2];
	int Color[3];
	Tower_S(int x = 0, int y = 0);
	void Draw(Mat &Screen);
	bool Reload(Serial * Data);
	int Ammo;
	//	private:
};


class getLego
{
public:
	Camera * cam;
	int Red;
	int Green;
	int Blue;
	int  position[3][10][2];
	int * passer;
	getLego(int * TR, int * TG, int *TB, Mat* Map, Camera* caddr);
	int get(int color, int number, int axis);
	void set(Tower_C * TowerC, Tower_T * TowerT, Tower_S * TowerS);
	void getLego::scan(Tower_C * TowerC, Tower_T * TowerT, Tower_S * TowerS);
};
