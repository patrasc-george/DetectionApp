#include <iostream>
#include <fstream>
#include<QPushButton>

#include <ObjectDetection.h>

class OBJECTDETECTION_API ObjectDetector : public Detector {
private:
	std::vector<std::pair<std::string, bool>> classNames;
	std::string classNamesPath;
	std::string infGraphPath;
	std::string framework;
	cv::Scalar meanValues;
	cv::dnn::Net model;
	bool showConfidence;
	float minConfidence = 0.6;
	cv::Rect lastRect;

public:
	/**
	 * @brief Constructs an ObjectDetector object.
	 * @details This constructor initializes an ObjectDetector object with the provided properties.
	 It sets the model path, class names path, inference graph path, framework,
	 whether or not to swap red and blue channels, mean values for normalization, and the type of detector (network).
	 * @param[in] props A detectorProperties object containing properties for the object detector.
	 */
	ObjectDetector(detectorProperties props);

	/**
	 * @brief Detects objects in an image and optionally draws confidence values on the image.
	 * @details This function detects objects in an image using a neural network model.
	 It preprocesses the image by creating a blob from it and then feeds it into the model.
	 The model outputs a matrix of detections, each row representing a detected object.
	 For each detection, it checks if the confidence value is above a minimum threshold.
	 If it is, it draws a rectangle around the detected object on the image and labels it with the class name and optionally the confidence value.
	 * @param[in,out] image The image to detect objects in.
	 * @param[in] showConf A boolean indicating whether or not to draw confidence values on the image.
	 */
	void detect(cv::Mat& image, bool showConf = false);

	/**
	 * @brief Sets the minimum confidence value for object detection.
	 * @details This function sets the minimum confidence value for object detection.
	 Only detections with a confidence value above this threshold will be considered valid and drawn on the image by the detect function.
	 The provided value must be between 0 and 1 (exclusive).
	 * @param[in] c The minimum confidence value to set.
	 */
	void setMinConfidence(float c);

	/**
	 * @brief Returns the last detected object rectangle.
	 * @details This function returns the last object rectangle that was detected by an ObjectDetector.
	 The specific behavior of this function depends on how it is implemented in a derived class of ObjectDetector.
	 * @return A cv::Rect representing the last detected object rectangle.
	 */
	cv::Rect getLastRect();

	/**
	 * @brief Initializes the ObjectDetector object.
	 * @details This function initializes an ObjectDetector object by loading its model and class names.
	 It checks if the model path and inference graph path are valid
	 and then attempts to load the model from those paths using the specified framework.
	 If successful, it also reads the class names from the class names file and stores them in a vector.
	 If any of these steps fail, it returns an error code indicating what went wrong.
	 If all steps are successful, it returns 1 to indicate success.
	 * @return An integer representing the success or failure of initialization.
	 */
	int init();

	void setClassNamesValues(const std::vector<QPushButton*>& classButtons);

	int getType() { return type; };
};