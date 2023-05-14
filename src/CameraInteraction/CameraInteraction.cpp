#include "CameraInteraction.h"

#include <opencv2/opencv.hpp>

/**
 * @brief Applies binary thresholding to an image.
 * @param image The image to be processed.
 * @param threshold The threshold value.
 */
void binaryThresholding(cv::Mat& image, short threshold)
{
	cv::threshold(image, image, threshold, 255, cv::THRESH_BINARY);
}

/**
 * @brief Applies zero thresholding to an image.
 * @param image The image to be processed.
 * @param threshold The threshold value.
 */
void zeroThresholding(cv::Mat& image, short threshold)
{
	cv::threshold(image, image, threshold, 255, cv::THRESH_TOZERO);
}

/**
 * @brief Applies adaptive thresholding to an image.
 * @param image The image to be processed.
 * @param threshold The threshold value.
 */
void adaptiveThresholding(cv::Mat& image, short threshold)
{
	cv::adaptiveThreshold(image, image, threshold, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 11, 2);
}

/**
 * @brief Equalizes the histogram of an image.
 * @param image The image to be processed.
 */
void histogramEqualization(cv::Mat& image)
{
	cv::equalizeHist(image, image);
}

/**
 * @brief Detects edges in an image using the Laplacian operator.
 * @param image The image to be processed.
 */
void detectEdges(cv::Mat& image)
{
	cv::Laplacian(image, image, CV_8U);
	cv::normalize(image, image, 0, 255, cv::NORM_MINMAX);
}

