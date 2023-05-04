#include "CameraInteraction.h"

#include <opencv2/opencv.hpp>

void displayInfo(cv::Mat& image, std::string key, std::string value, cv::Point pos) {
	std::stringstream ss;
	ss << key << ": " << value;
	putText(image, ss.str(), pos, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(150, 150, 20), 2);
}

void binaryThresholding(cv::Mat& image, short threshold)
{
	cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
	cv::threshold(image, image, threshold, 255, cv::THRESH_BINARY);
}

void histogramEqualization(cv::Mat& image)
{
	cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(image, image);
}