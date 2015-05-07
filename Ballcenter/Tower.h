
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
using namespace cv;

class Tower_C
{
public:
	int Damage;
	int Range;
	int Capacity;
	int Location[2];
	int Color[3];
	Tower_C(int x, int y);
	Tower_C();
	void Draw(Mat &Screen);
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
	Tower_T(int x, int y);
	Tower_T();
	void Draw(Mat &Screen);
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
	Tower_S(int x, int y);
	Tower_S();
	void Draw(Mat &Screen);
	//	private:
};


class getLego
{
public:
	int Red;
	int Green;
	int Blue;
	int  position[3][10][2];
	int * passer;
	getLego(int * TR, int * TG, int *TB, Mat* Map);
	int get(int color, int number, int axis);
	void set(Tower_C * TowerC, Tower_T * TowerT, Tower_S * TowerS);
};
