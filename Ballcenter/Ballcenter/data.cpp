#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "debug.h"
#include "const.h"
#include "data.h"

using namespace cv;

namespace data{
	inline float distance(float x2, float x1, float y2, float y1){
		return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	}

	int closest_circle(float x, float y){
		// TODO
		return (balls.size() > 0 ? 0 : -1);
	}

	void tick(std::vector<cv::Vec<float, 3>>& circles){
		// TODO: multiple balls
		for (auto &circle : circles){
			int index = closest_circle(circle[0], circle[1]);
			if (index < 0){
				balls.push_back(ball());
				index = 0;
			}
			balls[index].velocity = distance(circle[0], balls[index].x, circle[1], balls[index].y);
			balls[index].x = circle[0];
			balls[index].y = circle[1];
		}
	}
}