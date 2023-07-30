#include <ObjectDetection.h>

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
	virtual std::vector<std::string> getSortedClassNames() const { return std::vector<std::string>(); };
};