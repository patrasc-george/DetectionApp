#include "ImageProcessingUtils.h"

#include <opencv2/opencv.hpp>

/**
 * @brief Applies binary thresholding to an image.
 * @param[in,out] image The image to be processed.
 * @param[in] threshold The threshold value.
 * @details This function applies binary thresholding to an image using OpenCV's threshold function. It sets all pixel values below the specified threshold to 0 and all pixel values above the threshold to 255. The resulting image is a binary image where all pixels are either black or white.
 */
void binaryThresholding(cv::Mat& image, short threshold) {
	cv::threshold(image, image, threshold, 255, cv::THRESH_BINARY);
}

/**
 * @brief Applies zero thresholding to an image.
 * @param[in,out] image The image to be processed.
 * @param[in] threshold The threshold value.
 * @details This function applies zero thresholding to an image using OpenCV's threshold function. It sets all pixel values below the specified threshold to 0 and leaves all pixel values above the threshold unchanged. The resulting image is a grayscale image where all pixels below the threshold are black and all pixels above the threshold retain their original values.
 */
void zeroThresholding(cv::Mat& image, short threshold) {
	cv::threshold(image, image, threshold, 255, cv::THRESH_TOZERO);
}

/**
 * @brief Applies adaptive thresholding to an image.
 * @param[in,out] image The image to be processed.
 * @param[in] threshold The threshold value.
 * @details This function applies adaptive thresholding to an image using OpenCV's adaptiveThreshold function. It calculates a different threshold for each pixel based on its local neighborhood and then applies binary thresholding using that calculated threshold. The resulting image is a binary image where all pixels are either black or white. The size of the local neighborhood and the method used to calculate the thresholds can be adjusted by changing the parameters of the adaptiveThreshold function call.
 */
void adaptiveThresholding(cv::Mat& image, short threshold) {
	cv::adaptiveThreshold(image, image, threshold, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 11, 2);
}

/**
 * @brief Equalizes the histogram of an image.
 * @param[in,out] image The image to be processed.
 * @details This function equalizes the histogram of an image using OpenCV's equalizeHist function. It redistributes the pixel values in such a way that their histogram is approximately flat. This can improve the contrast of an image by stretching out its intensity range. The resulting image is a grayscale image with improved contrast.
 */
void histogramEqualization(cv::Mat& image) {
	cv::equalizeHist(image, image);
}

/**
 * @brief Detects edges in an image using the Laplacian operator.
 * @param[in,out] image The image to be processed.
 * @details This function detects edges in an image using OpenCV's Laplacian function. It applies the Laplacian operator to the image, which calculates the second derivative of its intensity values. This highlights regions of rapid intensity change, which correspond to edges in the original image. The resulting edge map is then normalized and scaled to fit within the 0-255 range so that it can be displayed as a grayscale image.
 */
void detectEdges(cv::Mat& image) {
	cv::Laplacian(image, image, CV_8U);
	cv::normalize(image, image, 0, 255, cv::NORM_MINMAX);
}

/**
 * @brief Sets the confidence value.
 * @param[in] val The confidence value to be set.
 */
void FrameOptions::setConfidence(const short& val) {
	confidence = val;
}

/**
 * @brief Gets the confidence value.
 * @return Returns the confidence value.
 */
short FrameOptions::getConfidence() const {
	return confidence;
}

/**
 * @brief Sets the threshold value.
 * @param[in] val The threshold value to be set.
 */
void FrameOptions::setThreshold(const short& val) {
	threshold = val;
}

/**
 * @brief Gets the threshold value.
 * @return Returns the threshold value.
 */
short FrameOptions::getThreshold() const {
	return threshold;
}

/**
 * @brief Sets whether to show features or not.
 * @param[in] val The boolean value to be set.
 */
void FrameOptions::setShowFeatures(const bool& val) {
	showFeatures = val;
}

/**
 * @brief Gets whether to show features or not.
 * @return Returns true if features are to be shown, otherwise returns false.
 */
bool FrameOptions::getShowFeatures() const {
	return showFeatures;
}

/**
 * @brief Sets whether to flip the image horizontally or not.
 * @param[in] val The boolean value to be set.
 */
void FrameOptions::setFlipH(const bool& val) {
	flipH = val;
}

/**
 * @brief Gets whether to flip the image horizontally or not.
 * @return Returns true if the image is to be flipped horizontally, otherwise returns false.
 */
bool FrameOptions::getFlipH() const {
	return flipH;
}

/**
 * @brief Sets whether to flip the image vertically or not.
 * @param[in] val The boolean value to be set.
 */
void FrameOptions::setFlipV(const bool& val) {
	flipV = val;
}

/**
 * @brief Gets whether to flip the image vertically or not.
 * @return Returns true if the image is to be flipped horizontally, otherwise returns false.
 */
bool FrameOptions::getFlipV() const {
	return flipV;
}

/**
 * @brief Sets whether to show confidence values or not.
 * @param[in] val The boolean value to be set.
 */
void FrameOptions::setShowConfidence(const bool& val) {
	showConfidence = val;
}

/**
 * @brief Gets whether to show confidence values or not.
 * @return Returns true if confidence values are to be shown, otherwise returns false.
 */
bool FrameOptions::getShowConfidence() const {
	return showConfidence;
}

/**
 * @brief Sets the binary thresholding value.
 * @param[in] val The binary thresholding value to be set.
 */
void FrameOptions::setBinaryThresholdingValue(const short& val) {
	binaryThresholdingValue = val;
}

/**
 * @brief Gets the binary thresholding value.
 * @return Returns the binary thresholding value.
 */
short FrameOptions::getBinaryThresholdingValue() const {
	return binaryThresholdingValue;
}

/**
 * @brief Sets the zero thresholding value.
 * @param[in] val The zero thresholding value to be set.
 */
void FrameOptions::setZeroThresholdingValue(const short& val) {
	zeroThresholdingValue = val;
}

/**
 * @brief Gets the zero thresholding value.
 * @return Returns the zero thresholding value.
 */
short FrameOptions::getZeroThresholdingValue() const {
	return zeroThresholdingValue;
}

/**
 * @brief Sets the adaptive thresholding value.
 * @param[in] val The adaptive thresholding value to be set.
 */
void FrameOptions::setAdaptiveThresholdingValue(const short& val) {
	adaptiveThresholdingValue = val;
}

/**
 * @brief Gets the adaptive thresholding value.
 * @return Returns the adaptive thresholding value.
 */
short FrameOptions::getAdaptiveThresholdingValue() const {
	return adaptiveThresholdingValue;
}

/**
 * @brief Sets whether to equalize the histogram of an image or not.
 * @param[in] val The boolean value to be set.
 */
void FrameOptions::setHistogramEqualization(const bool& val) {
	histogramEqualization = val;
}

/**
 * @brief Gets whether to equalize the histogram of an image or not.
 * @return Returns true if the histogram of an image is to be equalized, otherwise returns false.
 */
bool FrameOptions::getHistogramEqualization() const {
	return histogramEqualization;
}

/**
 * @brief Sets whether to detect edges in an image or not.
 * @param[in] val The boolean value to be set.
 */
void FrameOptions::setDetectEdges(const bool& val) {
	detectEdges = val;
}

/**
 * @brief Gets whether to detect edges in an image or not.
 * @return Returns true if edges are to be detected in an image, otherwise returns false.
 */
bool FrameOptions::getDetectEdges() const {
	return detectEdges;
}

/**
 * @brief Adds a new option to the history.
 * @param[in] prop The option to be added.
 * @param[in] value The value of the option to be added.
 * @details This function adds a new option to the history of options. It first checks if the undo stack is full (i.e., has reached its maximum size of 10). If it is, it removes the oldest option from the stack. It then adds the current status to the undo stack and updates the current status with the provided option and value. If there are any options in the redo stack, it clears them. This function can be called whenever an option is changed by the user to keep track of its previous values and allow for undo/redo functionality.
 */
void OptionsHistory::add(revertable_options prop, short value) {
	if (undoStack.size() >= 10)
		undoStack.pop_front();
	undoStack.push_back(currentStatus);
	switch (prop) {
	case FLIP_HORIZONTAL:
		currentStatus.setFlipH(value);
		break;
	case FLIP_VERTICAL:
		currentStatus.setFlipV(value);
		break;
	case SHOW_FEATURES:
		currentStatus.setShowFeatures(value);
		break;
	case SHOW_CONFIDENCE:
		currentStatus.setShowConfidence(value);
		break;
	case CONFIDENCE:
		// not implemented
		currentStatus.setShowConfidence(value);
		break;
	case THRESHOLD:
		// not implemented
		currentStatus.setThreshold(value);
		break;
	case BINARY_THRESHOLDING:
		currentStatus.setBinaryThresholdingValue(value);
		break;
	case ZERO_THRESHOLDING:
		currentStatus.setZeroThresholdingValue(value);
		break;
	case ADAPTIVE_THRESHOLDING:
		currentStatus.setAdaptiveThresholdingValue(value);
		break;
	case HISTOGRAM_EQUALIZATION:
		currentStatus.setHistogramEqualization(value);
		break;
	case DETECT_EDGES:
		currentStatus.setDetectEdges(value);
		break;
	default:
		return;
	}
	lastChangedOption = prop;
	currentStatus.lastChangedOption = lastChangedOption;
	if (!redoStack.empty())
		redoStack.clear();
}

/**
 * @brief Checks if redo is possible.
 * @return Returns true if redo is possible, otherwise returns false.
 * @details This function checks if redo is possible by checking if the redo stack is not empty. If it is not empty, it means that there are options that can be redone and it returns true. Otherwise, it returns false to indicate that redo is not possible.
 */
bool OptionsHistory::canRedo() {
	return !redoStack.empty();
}

/**
 * @brief Checks if undo is possible.
 * @return Returns true if undo is possible, otherwise returns false.
 * @details This function checks if undo is possible by checking if the undo stack is not empty. If it is not empty, it means that there are options that can be undone and it returns true. Otherwise, it returns false to indicate that undo is not possible.
 */
bool OptionsHistory::canUndo() {
	return !undoStack.empty();
}

/**
 * @brief Resets the history.
 * @details This function resets the history by clearing both the undo and redo stacks. It can be called whenever the history needs to be reset (e.g., when loading a new image).
 */
void OptionsHistory::reset() {
	undoStack.clear();
	redoStack.clear();
}

/**
 * @brief Undoes the last change.
 * @details This function undoes the last change by moving the current status from the undo stack to the redo stack and setting the current status to the previous status stored in the undo stack. It can be called whenever an undo operation is requested by the user (e.g., when they click on an undo button).
 */
void OptionsHistory::undo() {
	if (!undoStack.empty()) {
		lastChangedOption = currentStatus.lastChangedOption;
		redoStack.push_back(currentStatus);
		currentStatus = undoStack.back();
		undoStack.pop_back();
	}
}

/**
 * @brief Redoes the last change.
 * @details This function redoes the last change by moving the current status from the redo stack to the undo stack and setting the current status to the previous status stored in the redo stack. It can be called whenever a redo operation is requested by the user (e.g., when they click on a redo button).
 */
void OptionsHistory::redo() {
	if (!redoStack.empty()) {
		lastChangedOption = redoStack.back().lastChangedOption;
		undoStack.push_back(currentStatus);
		currentStatus = redoStack.back();
		redoStack.pop_back();
	}
}

/**
 * @brief Gets the current status.
 * @return Returns a pointer to the current status.
 * @details This function returns a pointer to the current status. It can be called at any time to get a reference to the current status and access its properties or methods.
 */
FrameOptions* OptionsHistory::get() {
	return &currentStatus;
}

/**
 * @brief Returns a string describing the last changed option.
 * @return A string describing the last changed option.
 * @details This function returns a string describing the last changed option. It can be called at any time to get a human-readable description of the last option that was changed by the user.
 */
std::string OptionsHistory::lastChange() {
	switch (lastChangedOption) {
	case FLIP_HORIZONTAL:
		return "horizontal flip";
	case FLIP_VERTICAL:
		return "vertical flip";
	case SHOW_FEATURES:
		return "show features";
	case SHOW_CONFIDENCE:
		return "show confidence";
	case THRESHOLD:
		return "minimum threshold change";
	case CONFIDENCE:
		return "minimum confidence change";
	case BINARY_THRESHOLDING:
		return "binary thresholding";
	case ZERO_THRESHOLDING:
		return "zero thresholding";
	case ADAPTIVE_THRESHOLDING:
		return "adaptive thresholding";
	case HISTOGRAM_EQUALIZATION:
		return "histogram equalization";
	case DETECT_EDGES:
		return "edges detection";
	default:
		return "last action";
	}
}

/**
 * @brief Constructs a Timer object and starts the timer.
 * @param[out] c An integer reference to be used as a counter. (output)
 * @details This constructor initializes a Timer object and starts its internal timer. It also stores a reference to an integer counter that will be updated with the elapsed time when the Timer object is destroyed. This constructor can be called at any time to start timing an operation (e.g., when processing an image).
 */
Timer::Timer(int& c) {
	start = std::chrono::steady_clock::now();
	duration = start - start;
	counter = &c;
}

/**
 * @brief Destroys the Timer object and stops the timer.
 * @details This destructor stops the internal timer of a Timer object and calculates its duration. It then updates the integer counter that was provided when constructing the Timer object with the elapsed time (in seconds). This destructor is called automatically when a Timer object goes out of scope or is deleted (e.g., when an image processing operation completes).
 */
Timer::~Timer() {
	end = std::chrono::steady_clock::now();
	duration = end - start;
	*counter = 1 / duration.count();
}