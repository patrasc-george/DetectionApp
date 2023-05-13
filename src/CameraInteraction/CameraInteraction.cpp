#include "CameraInteraction.h"

#include <opencv2/opencv.hpp>

void binaryThresholding(cv::Mat& image, short threshold)
{
	cv::threshold(image, image, threshold, 255, cv::THRESH_BINARY);
}
void zeroThresholding(cv::Mat& image, short threshold)
{
	cv::threshold(image, image, threshold, 255, cv::THRESH_TOZERO);
}
void adaptiveThresholding(cv::Mat& image, short threshold)
{
	cv::adaptiveThreshold(image, image, threshold, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 11, 2);
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


void FrameOptions::setConfidence(const short& val) {
	confidence = val;
}
short FrameOptions::getConfidence() const {
	return confidence;
}

void FrameOptions::setThreshold(const short& val) {
	threshold = val;
}
short FrameOptions::getThreshold() const {
	return threshold;
}

void FrameOptions::setShowFeatures(const bool& val) {
	showFeatures = val;
}
bool FrameOptions::getShowFeatures() const {
	return showFeatures;
}

void FrameOptions::setFlipH(const bool& val) {
	flipH = val;
}
bool FrameOptions::getFlipH() const {
	return flipH;
}

void FrameOptions::setFlipV(const bool& val) {
	flipV = val;
}
bool FrameOptions::getFlipV() const {
	return flipV;
}

void FrameOptions::setShowConfidence(const bool& val) {
	showConfidence = val;
}
bool FrameOptions::getShowConfidence() const {
	return showConfidence;
}

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
	default:
		return;
	}
	lastChangedOption = prop;
	currentStatus.lastChangedOption = lastChangedOption;
	if (!redoStack.empty())
		redoStack.clear();
}
bool OptionsHistory::canRedo() {
	return !redoStack.empty();
}
bool OptionsHistory::canUndo() {
	return !undoStack.empty();
}
void OptionsHistory::reset() {
	undoStack.clear();
	redoStack.clear();
}
void OptionsHistory::undo() {
	if (!undoStack.empty()) {
		lastChangedOption = currentStatus.lastChangedOption;
		redoStack.push_back(currentStatus);
		currentStatus = undoStack.back();
		undoStack.pop_back();
	}
}
void OptionsHistory::redo() {
	if (!redoStack.empty()) {
		lastChangedOption = redoStack.back().lastChangedOption;
		undoStack.push_back(currentStatus);
		currentStatus = redoStack.back();
		redoStack.pop_back();
	}
}
FrameOptions* OptionsHistory::get() {
	return &currentStatus;
}

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
	default:
		return "last action";
	}
}

Timer::Timer(int& c) {
	start = std::chrono::steady_clock::now();
	duration = start - start;
	counter = &c;
}
Timer::~Timer() {
	end = std::chrono::steady_clock::now();
	duration = end - start;
	*counter = 1 / duration.count();
}