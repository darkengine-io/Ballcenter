#pragma once

struct ball {
	cv::Point position;
	float velocity;
};

namespace data {
	static std::vector<ball> balls;
	void tick(std::vector<cv::Vec<float, 3>>& circles);
}