#include "ImageProcessingUtils.h"

#include <opencv2/opencv.hpp>

/**
 * @brief Applies binary thresholding to an image.
 * @param[in,out] image The image to be processed.
 * @param[in] threshold The threshold value.
 */
void binaryThresholding(cv::Mat& image, short threshold) {
	cv::threshold(image, image, threshold, 255, cv::THRESH_BINARY);
}

/**
 * @brief Applies zero thresholding to an image.
 * @param[in,out] image The image to be processed.
 * @param[in] threshold The threshold value.
 */
void zeroThresholding(cv::Mat& image, short threshold) {
	cv::threshold(image, image, threshold, 255, cv::THRESH_TOZERO);
}

/**
 * @brief Applies adaptive thresholding to an image.
 * @param[in,out] image The image to be processed.
 * @param[in] threshold The threshold value.
 */
void adaptiveThresholding(cv::Mat& image, short threshold) {
	cv::adaptiveThreshold(image, image, threshold, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 11, 2);
}

/**
 * @brief Equalizes the histogram of an image.
 * @param[in,out] image The image to be processed.
 */
void histogramEqualization(cv::Mat& image) {
	cv::equalizeHist(image, image);
}

/**
 * @brief Detects edges in an image using the Laplacian operator.
 * @param[in,out] image The image to be processed.
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
 */
bool OptionsHistory::canRedo() {
	return !redoStack.empty();
}

/**
 * @brief Checks if undo is possible.
 * @return Returns true if undo is possible, otherwise returns false.
 */
bool OptionsHistory::canUndo() {
	return !undoStack.empty();
}

/**
 * @brief Resets the history.
 */
void OptionsHistory::reset() {
	undoStack.clear();
	redoStack.clear();
}

/**
 * @brief Undoes the last change.
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
 */
FrameOptions* OptionsHistory::get() {
	return &currentStatus;
}

/**
 * @brief Returns a string describing the last changed option.
 * @return A string describing the last changed option.
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
 */
Timer::Timer(int& c) {
	start = std::chrono::steady_clock::now();
	duration = start - start;
	counter = &c;
}

/**
 * @brief Destroys the Timer object and stops the timer.
 */
Timer::~Timer() {
	end = std::chrono::steady_clock::now();
	duration = end - start;
	*counter = 1 / duration.count();
}