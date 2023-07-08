#pragma once
#include <opencv2/opencv.hpp>

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

enum DetectorType {
	cascade, network
};

struct detectorProperties {
	std::string modelPath;
	std::string classNamesPath;
	std::string infGraphPath;
	std::string framework;
	cv::Scalar meanValues;
	bool shouldSwapRB;

	detectorProperties()
		: modelPath("\0"), 
		classNamesPath("\0"), 
		infGraphPath("\0"), 
		framework("\0"), 
		shouldSwapRB(true), 
		meanValues(cv::Scalar(0,0,0)) {}
};

class Detector {
protected:
	std::string modelPath;
	bool shouldSwapRB = false;
	DetectorType type;

public:
	std::string currentClassName = "";

public:
	virtual void detect(cv::Mat& image, bool = false) = 0;
	virtual cv::Rect getLastRect() = 0;
	virtual int init() = 0;
	virtual void setMinConfidence(float c) {};

	virtual bool canDetectEyes() { return false; };
	virtual bool canDetectSmiles() { return false; };
	virtual int getType() { return 0; };
	virtual ~Detector() { std::cout << "detector destroyed" << std::endl; }
};

/**
 * @brief Draws a label on an image.
 * @details This function draws a text label on an image using OpenCV's putText function.
 The label is drawn with a specific font, size, color and thickness.
 The position of the label is determined by the provided left and top coordinates.
 If the top coordinate is less than the height of the label, it is adjusted to ensure that the label is fully visible on the image.
 * @param[in,out] image The image to draw the label on.
 * @param[in] label The text of the label to draw.
 * @param[in] left The x-coordinate of the left side of the label.
 * @param[in] top The y-coordinate of the top side of the label.
 */
void drawLabel(cv::Mat & image, std::string label, int left, int top);