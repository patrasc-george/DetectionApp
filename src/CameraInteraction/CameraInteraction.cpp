#include "CameraInteraction.h"

#include <opencv2/opencv.hpp>

void binaryThresholding(cv::Mat& image, short threshold)
{
	cv::threshold(image, image, threshold, 255, cv::THRESH_BINARY);
}

void histogramEqualization(cv::Mat& image)
{
	cv::equalizeHist(image, image);
}

void detectEdges(cv::Mat& image)
{
	cv::Laplacian(image, image, CV_8U);
	cv::normalize(image, image, 0, 255, cv::NORM_MINMAX);
}
