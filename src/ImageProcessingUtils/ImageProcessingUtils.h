#pragma once
#include "OptionsHistory.h"
#include "Timer.h"

#include <opencv2/opencv.hpp>
#include <QPainter>

class IMAGEPROCESSINGUTILS_API ProcessingAlgorithms
{
public:
	/**
	 * @brief Applies binary thresholding to an image.
	 * @details	It sets all pixel values below the specified threshold to 0 and all pixel values above the threshold to 255.
	 The resulting image is a binary image where all pixels are either black or white.
	 * @param[in] src The source image
	 * @param[out] dst The destination image
	 * @param[in] threshold The threshold value.
	 */
	static void binaryThresholding(cv::Mat src, cv::Mat& dst, short threshold);

	/**
	 * @brief Applies zero thresholding to an image.
	 * @details It sets all pixel values below the specified threshold to 0 and leaves all pixel values above the threshold unchanged.
	 The resulting image is an image where all pixels below the threshold are black
	 and all pixels above the threshold retain their original values.
	 Each color channel is computed separately.
	 * @param[in] src The source image
	 * @param[out] dst The destination image
	 * @param[in] threshold The threshold value.
	 */
	static void zeroThresholding(cv::Mat src, cv::Mat& dst, short threshold);

	/**
	 * @brief Applies adaptive thresholding to an image.
	 * @details This function applies adaptive thresholding to an image using OpenCV's adaptiveThreshold function.
	 It calculates a different threshold for each pixel based on its local neighborhood
	 and then applies binary thresholding using that calculated threshold.
	 The resulting image is a binary image where all pixels are either black or white.
	 The size of the local neighborhood and the method used to calculate the thresholds can be adjusted
	 by changing the parameters of the adaptiveThreshold function call.
	 * @param[in] src The source image
	 * @param[out] dst The destination image
	 * @param[in] threshold The threshold value.
	 */
	static void adaptiveThresholding(cv::Mat src, cv::Mat& dst, short threshold);

	/**
	* * @brief Applies truncate thresholding to an image.
	* @details It sets all pixels above a specified threshold to that threshold value and leaves the pixels with a luminance
	below the threshold unchanged.
	* @param [in] src The source image
	* @param[out] dst The destination image
	*/
	static void truncate(cv::Mat src, cv::Mat& dst, short threshold);

	/**
	 * @brief Equalizes the histogram of an image.
	 * @details This function equalizes the histogram of an image using OpenCV's equalizeHist function.
	 It redistributes the pixel values in such a way that their histogram is approximately flat.
	 This can improve the contrast of an image by stretching out its intensity range.
	 The resulting image is a grayscale image with improved contrast.
	 * @param[in,out] image The image to be processed.
	 */
	static void histogramEqualization(cv::Mat src, cv::Mat& dst);

	/**
	 * @brief Detects edges in an image using the Laplacian operator.
	 * @details This function detects edges in an image using OpenCV's Laplacian function.
	 It applies the Laplacian operator to the image, which calculates the second derivative of its intensity values.
	 This highlights regions of rapid intensity change, which correspond to edges in the original image.
	 The resulting edge map is then normalized and scaled to fit within the 0-255 range so that it can be displayed as a grayscale image.
	 * @param[in,out] image The image to be processed.
	 */
	static void detectEdges(cv::Mat src, cv::Mat& dst);

	/**
	* @brief Applies various image processing algorithms based on the provided options.
	* @details This function applies a series of image processing algorithms to the input image
	based on the specified options. The algorithms include histogram equalization, binary thresholding,
	adaptive thresholding, zero thresholding, edge detection, and truncation.
	* @param[in,out] image The input and output cv::Mat image to be processed.
	* @param[in] options A pointer to a FrameOptions object containing the algorithm activation flags.
	* @param[in] value The threshold value used in some of the algorithms (e.g., binary thresholding).
	*/
	static void applyingAlgorithms(cv::Mat& image, FrameOptions* options, const short& value);
};

/**
 * @brief Converts a cv::Mat to a QImage.
 * @details This function converts a cv::Mat to a QImage.
 It supports 8-bit, 4 channel; 8-bit, 3 channel; and 8-bit, 1 channel cv::Mats.
 If the cv::Mat type is not supported, the function returns false and prints an error message to the console.
 * @param[in] src The source cv::Mat to convert.
 * @param[out] dest The destination QImage to store the converted image.
 * @return Returns true if the conversion was successful, otherwise returns false.
 */
bool IMAGEPROCESSINGUTILS_API ConvertMat2QImage(const cv::Mat& src, QImage& dest);

/**
 * @brief Converts a QImage to a cv::Mat.
 * @details This function converts a QImage to a cv::Mat.
 It supports Format_ARGB32, Format_ARGB32_Premultiplied, Format_RGB32, Format_RGB888 and Format_Indexed8 QImages.
 If the QImage format is not supported, the function returns false and prints an error message to the console.
 * @param[in] src The source QImage to convert.
 * @param[out] dest The destination cv::Mat to store the converted image.
 * @return Returns true if the conversion was successful, otherwise returns false.
 */
bool IMAGEPROCESSINGUTILS_API ConvertQImage2Mat(const QImage& src, cv::Mat& dest);

/**
 * @brief Generates an image with a logo and a text overlay.
 * @details This function creates a QImage with a logo centered on a white background.
 * It also adds a text overlay below the logo to indicate that the camera is turned off.
 * @param[in] width The width of the generated image.
 * @param[in] height The height of the generated image.
 * @return Returns a QImage with the logo and text overlay.
 */
QImage IMAGEPROCESSINGUTILS_API putLogo(const short& width, const short& height);