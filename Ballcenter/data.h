#pragma once

struct ball {
	float x, y, velocity;
};

namespace data {
	static std::vector<ball> balls;
	void tick(std::vector<cv::Vec<float, 3>>& circles);
}