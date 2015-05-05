#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>

using namespace cv;

int const U = 1;
int const D = 3;
int const L = 4;
int const R = 2;

class Target
{
	public:
	int speed;
	int health;
	int Location[2];
	char path(int *x, int *y, int t, char type, char Dir, int *alive, int inity, int initx, int *wave, int *gp, int * speed, int *targetHit, int *HD, Mat * Out);
	Target(int wave , int HP);
	void Draw(Mat Screen, int x, int y , char Dir);
	void Hit(Mat Screen, int x, int y, int num , char Dir);
//	private:
};
