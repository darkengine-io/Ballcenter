#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <math.h>
#include <cstdio>
#include <opencv/cv.h>
#include <sstream>

#include "const.h"
#include "Tower.h"
#include "Target.h"

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()


using namespace cv;
using namespace std;


int main( int argc, char** argv )
{
	int gp = 20;
	int wave = 1;
	int hit = 0;
	int TR;
	int TG;
	int TB;
	getLego start(&TR, &TG, &TB);
	int TN = TR + TG + TB;
	int speed = 1;
	char Dir = 'R';
	Tower_C * TowerC = new Tower_C [TR];
	Tower_T * TowerT = new Tower_T [TG];
	Tower_S * TowerS = new Tower_S [TB];
	start.set(TowerC,TowerT,TowerS);
	if( argc < 2)
	{
		cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
	}
	Size Screen(OUT_WIDTH, OUT_HEIGHT);
	Mat Map;
	Mat Out;


	//Size Screen (800,600);
	

	cvNamedWindow(GAME_WIN, CV_WINDOW_NORMAL);
	cvSetWindowProperty(GAME_WIN, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	Map = imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);   // Read the file
	if(! Map.data )                              // Check for invalid input
	{
		cout <<  "Could not open or find the Map" << std::endl ;
		return -1;
	}
	int initx = 11;
	int inity = 240;
	int x = initx;
	int y = inity;
	int step = 0;
	int alive = 1;
	int targetHit = 0;
	int HD = 0;
	int Start;

	for(int i = 0;i > -1 ;i++)
	{
		if(char(cvWaitKey(10)) ==27)
			break;
		Map = imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);
		Out = Map;
		if (wave > 20)
			//speed = wave / 10;
			speed = 1;
		Target tank(wave,targetHit);
		Dir = tank.path(&x, &y, 1, 1, Dir, &alive, inity, initx, &wave, &gp, &speed, &targetHit, &HD, &Out);
		cout << "Direction: " << Dir << endl;
		cout << x << "," << y << endl;
		Point textOrg(x,y);
		Point wavep(300,80);
		Point LL(280,40);
		if(alive)
		{
			if(targetHit > 39)
			{
				targetHit = 40;
				tank.Hit(Map,x,y,HD,Dir);
				if (HD == 5)
					alive = 0;
				else
					HD++;
			}
			else
				tank.Draw(Map,x,y,Dir);
		}
		//tunnel
		Mat tunnel = imread("s-e.png", CV_LOAD_IMAGE_UNCHANGED);
		resize(tunnel,tunnel,Size(70,50));
		cv::Rect roit( Point(0,220), tunnel.size());
		tunnel.copyTo(Map(roit));
		cv::Rect roitt( Point(529,180), tunnel.size());
		tunnel.copyTo(Map(roitt));
		//;
		// Tower
			//Red
			for(int count = 0; count < TR; count++)
			{
				int temp_val = pow((tank.Location[0] + 12 - TowerC[count].Location[0]),2) + pow((tank.Location[1] + 11 - TowerC[count].Location[1]),2) ;
                        	cout << "0x100" << temp_val;
				if((temp_val < (TowerC[count].Range * TowerC[count].Range) & ((x+y)%3 == 0)))
					line(Out,Point(tank.Location[0] + 12,tank.Location[1] + 12),Point(TowerC[count].Location[0],TowerC[count].Location[1]), cv::Scalar(0,222,255), 1, 1);
				if( (temp_val < (TowerC[count].Range * TowerC[count].Range) & (hit > wave-5)))
				{
					hit = 0;
					targetHit = targetHit + TowerC[count].Damage;
				}
				else
				{
					hit++;
				}
				cout << endl << hit << endl;
			}
		for(int count = 0; count < TR; count++)
		{
			TowerC[count].Draw(Out);
		}
			//Green
			for(int count = 0; count < TG; count++)
			{
				int temp_val = pow((tank.Location[0] + 12 - TowerT[count].Location[0]),2) + pow((tank.Location[1] + 11 - TowerT[count].Location[1]),2) ;
                        	cout << "0x100" << temp_val;
				if((temp_val < (TowerT[count].Range * TowerT[count].Range) & ((x+y)%3 == 0)))
					line(Out,Point(tank.Location[0] + 12,tank.Location[1] + 12),Point(TowerT[count].Location[0],TowerT[count].Location[1]), cv::Scalar(0,222,255), 1, 1);
				if( (temp_val < (TowerT[count].Range * TowerT[count].Range) & (hit > wave-5)))
				{
					hit = 0;
					targetHit = targetHit + TowerT[count].Damage;
				}
				else
				{
					hit++;
				}
				cout << endl << hit << endl;
			}
		for(int count = 0; count < TG; count++)
		{
			TowerT[count].Draw(Out);
		}
			//Blue
			for(int count = 0; count < TB; count++)
			{
				int temp_val = pow((tank.Location[0] + 12 - TowerS[count].Location[0]),2) + pow((tank.Location[1] + 11 - TowerS[count].Location[1]),2) ;
                        	cout << "0x100" << temp_val;
				if((temp_val < (TowerS[count].Range * TowerS[count].Range) & ((x+y)%3 == 0)))
					line(Out,Point(tank.Location[0] + 12,tank.Location[1] + 12),Point(TowerS[count].Location[0],TowerS[count].Location[1]), cv::Scalar(0,222,255), 1, 1);
				if( (temp_val < (TowerS[count].Range * TowerS[count].Range) & (hit > wave-5)))
				{
					hit = 0;
					targetHit = targetHit + TowerS[count].Damage;
				}
				else
				{
					hit++;
				}
				cout << endl << hit << endl;
			}
		for(int count = 0; count < TB; count++)
		{
			TowerS[count].Draw(Out);
		}

		putText(Out, "wave: " + SSTR(wave), wavep, cv::FONT_HERSHEY_DUPLEX, 1,Scalar::all(255), 2, 3);
		putText(Out, "Life left: " + SSTR(gp), LL, cv::FONT_HERSHEY_DUPLEX, 1,Scalar::all(255), 2, 3);
		resize(Out,Out,Screen);
		imshow(GAME_WIN, Out );                   // Show our image inside it.

	}
	waitKey(0);                                          // Wait for a keystroke in the window
	cout << "Game Over!" <<endl;
	return 0;
}
