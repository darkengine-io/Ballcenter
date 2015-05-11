#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "circles.h"
#include "debug.h"
#include "const.h"
#include "camera.h"

using namespace cv;

bool Camera::open(int id){
	cam = VideoCapture(id);
	cam.set(CV_CAP_PROP_FRAME_HEIGHT, IN_HEIGHT);
	cam.set(CV_CAP_PROP_FRAME_WIDTH, IN_WIDTH);
	if (!cam.isOpened()){
		return 0;
	}
	else{
		return 1;
	}
}
void Camera::get_frame(){
	cam >> src;
	src = src(aoi);
}
void Camera::get_raw_frame(){
	cam >> src;
}