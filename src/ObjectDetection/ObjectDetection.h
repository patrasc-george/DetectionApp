#pragma once
#include <opencv2/opencv.hpp>

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

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

struct detectorProperties {
	std::string modelPath;
	std::string classNamesPath;
	std::string infGraphPath;
	std::string framework;
	bool shouldSwapRB;
	cv::Scalar meanValues;
	detectorProperties()
		: modelPath("\0"), classNamesPath("\0"), infGraphPath("\0"), framework("\0"), shouldSwapRB(true), meanValues(cv::Scalar(0,0,0)) {}
};
enum DetectorType {
	cascade, network
};

class Detector {
protected:
	std::string modelPath;
	bool shouldSwapRB = false;
	DetectorType type;

public:
	virtual void detect(cv::Mat& image, bool = false) = 0;
	virtual void setMinConfidence(float c) {};
	virtual bool canDetectEyes() { return false; };
	virtual bool canDetectSmiles() { return false; };
	virtual int getType() { return 0; };
	virtual cv::Rect getLastRect() = 0;
	std::string currentClassName = "";
	virtual int init() = 0;
	virtual ~Detector() { std::cout << "detector destroyed" << std::endl; }

};

class OBJECTDETECTION_API FaceDetector : public Detector {
private:
	std::string eyeClassifierPath;
	std::string smileClassifierPath;
	cv::CascadeClassifier faceClassifier;
	cv::CascadeClassifier eyeClassifier;
	cv::CascadeClassifier smileClassifier;
	std::vector<cv::Rect> facesInFrame;
	std::vector<cv::Rect> eyes;
	std::vector<cv::Rect> smiles;
	bool eyesClassifierLoaded;
	bool smileClassifierLoaded;

public:
	/**
	 * @brief Constructs a FaceDetector object.
	 * @details This constructor initializes a FaceDetector object with the provided properties.
	 It sets the current class name to "face" and sets the model path,
	 whether or not to swap red and blue channels, and the type of detector (cascade).
	 It also stores the paths to the eye and smile classifier files for later use during initialization.
	 * @param[in] props A detectorProperties object containing properties for the face detector.
	 * @param[in] eyeClassifierPath A string representing the path to the eye classifier file.
	 * @param[in] smileClassifierPath A string representing the path to the smile classifier file.
	 */
	FaceDetector(detectorProperties& props, std::string eyeClassifierPath = "\0", std::string smileClassifierPath = "\0");

	/**
	 * @brief Detects faces in an image and optionally draws features on the image.
	 * @details This function detects faces in an image using a face classifier.
	 If the showFeatures parameter is true, it also attempts to detect eyes and smiles
	 within each detected face using eye and smile classifiers.
	 If any faces are detected, it draws rectangles around them on the image and labels them with the current class name.
	 If showFeatures is true and any eyes or smiles are detected,
	 it also draws circles around the eyes and rectangles around the smiles on the image.
	 * @param[in,out] image The image to detect faces in.
	 * @param[in] showFeatures A boolean indicating whether or not to draw features on the image.
	 */
	void detect(cv::Mat& image, bool showFeatures = false);

	/**
	 * @brief Returns the last detected face rectangle.
	 * @details This function returns the last face rectangle that was detected by the detect function.
	 If no faces were detected in the last call to detect, it returns an empty cv::Rect.
	 * @return A cv::Rect representing the last detected face rectangle.
	 */
	cv::Rect getLastRect();

	/**
	 * @brief Initializes the FaceDetector object.
	 * @details This function initializes a FaceDetector object by loading its classifiers.
	 It checks if the model path is valid and then attempts to load the face classifier from that path.
	 If successful, it also attempts to load the eye and smile classifiers if their paths were provided.
	 If any of these steps fail, it returns an error code indicating what went wrong.
	 If all steps are successful, it returns 1 to indicate success.
	 * @return An integer representing the success or failure of initialization.
	 */
	int init();

	bool canDetectEyes() { return eyesClassifierLoaded; };
	bool canDetectSmiles() { return smileClassifierLoaded; };
	int getType() { return type; };
};

class OBJECTDETECTION_API ObjectDetector : public Detector {
private:
	std::vector<std::string> classNames;
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

	int getType() { return type; };
};