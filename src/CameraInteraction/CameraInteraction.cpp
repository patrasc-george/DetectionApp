#include "CameraInteraction.h"

#include <iostream>
#include <opencv2/opencv.hpp>

void displayInfo(cv::Mat& image, std::string key, std::string value, cv::Point pos) {
	std::stringstream ss;
	ss << key << ": " << value;
	putText(image, ss.str(), pos, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(150, 150, 20), 2);
}