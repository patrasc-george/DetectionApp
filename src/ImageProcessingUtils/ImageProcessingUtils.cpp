#include "ImageProcessingUtils.h"

#include <opencv2/opencv.hpp>
#include <QImage>

void ProcessingAlgorithms::binaryThresholding(cv::Mat& image, short threshold) {
	if (image.type() != CV_8UC1)
		cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
	cv::threshold(image, image, threshold, 255, cv::THRESH_BINARY);
	cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
}

void ProcessingAlgorithms::zeroThresholding(cv::Mat& image, short threshold) {
	if (image.type() == CV_8UC1)
		cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
	if (image.type() == CV_8UC4)
		cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);
	cv::threshold(image, image, threshold, 255, cv::THRESH_TOZERO);
}

void ProcessingAlgorithms::adaptiveThresholding(cv::Mat& image, short threshold) {
	if (image.type() != CV_8UC1)
		cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
	cv::adaptiveThreshold(image, image, threshold, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2);
	cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
}

void ProcessingAlgorithms::histogramEqualization(cv::Mat& image) {
	if (image.type() != CV_8UC1)
		cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(image, image);
	cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
}

void ProcessingAlgorithms::detectEdges(cv::Mat& image) {
	if (image.type() == CV_8UC4)
		cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);
	cv::Laplacian(image, image, CV_8U);
	cv::normalize(image, image, 0, 255, cv::NORM_MINMAX);
}

void ProcessingAlgorithms::applyingAlgorithms(cv::Mat& image, FrameOptions* options, const short& value)
{
	if (options->getHistogramEqualization())
		histogramEqualization(image);
	if (options->getBinaryThresholdingValue())
		binaryThresholding(image, value);
	if (options->getAdaptiveThresholdingValue())
		adaptiveThresholding(image, value);
	if (options->getZeroThresholdingValue())
		zeroThresholding(image, value);
	if (options->getDetectEdges())
		detectEdges(image);
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

void FrameOptions::setBinaryThresholdingValue(const short& val) {
	binaryThresholdingValue = val;
}

short FrameOptions::getBinaryThresholdingValue() const {
	return binaryThresholdingValue;
}

void FrameOptions::setZeroThresholdingValue(const short& val) {
	zeroThresholdingValue = val;
}

short FrameOptions::getZeroThresholdingValue() const {
	return zeroThresholdingValue;
}

void FrameOptions::setAdaptiveThresholdingValue(const short& val) {
	adaptiveThresholdingValue = val;
}

short FrameOptions::getAdaptiveThresholdingValue() const {
	return adaptiveThresholdingValue;
}

void FrameOptions::setHistogramEqualization(const bool& val) {
	histogramEqualization = val;
}

bool FrameOptions::getHistogramEqualization() const {
	return histogramEqualization;
}

void FrameOptions::setDetectEdges(const bool& val) {
	detectEdges = val;
}

bool FrameOptions::getDetectEdges() const {
	return detectEdges;
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

bool ConvertMat2QImage(const cv::Mat& src, QImage& dest) {
	switch (src.type()) {
	// 8-bit, 4 channel
	case CV_8UC4:
		dest = QImage(src.data, src.cols, src.rows, static_cast<int>(src.step), QImage::Format_ARGB32);
		return true;

	// 8-bit, 3 channel
	case CV_8UC3:
		dest = QImage(src.data, src.cols, src.rows, static_cast<int>(src.step), QImage::Format_RGB888).rgbSwapped();
		return true;

	// 8-bit, 1 channel
	case CV_8UC1:
		dest =QImage(src.data, src.cols, src.rows, static_cast<int>(src.step), QImage::Format_Grayscale8);
		return true;
	default:
		std::cout << "ConvertMat2QImage() - cv::Mat image type not handled in switch:" << src.type() << std::endl;
		return false;
	}
}

bool ConvertQImage2Mat(const QImage& src, cv::Mat& dest) {
	switch (src.format()) {
	// 8-bit, 4 channel
	case QImage::Format_ARGB32:
	case QImage::Format_ARGB32_Premultiplied:
		dest = cv::Mat(src.height(), src.width(), CV_8UC4, const_cast<uchar*>(src.bits()), static_cast<size_t>(src.bytesPerLine()));
		return true;

	// 8-bit, 3 channel
	case QImage::Format_RGB32:
		dest = cv::Mat (src.height(), src.width(), CV_8UC4, const_cast<uchar*>(src.bits()), static_cast<size_t>(src.bytesPerLine()));

		cv::cvtColor(dest, dest, cv::COLOR_BGRA2BGR);   // drop the all-white alpha channel
		return true;

	// 8-bit, 3 channel
	case QImage::Format_RGB888:

		dest = cv::Mat(src.height(), src.width(), CV_8UC3, const_cast<uchar*>(src.rgbSwapped().bits()), static_cast<size_t>(src.rgbSwapped().bytesPerLine())).clone();
		return true;

	// 8-bit, 1 channel
	case QImage::Format_Indexed8:
		dest = cv::Mat (src.height(), src.width(), CV_8UC1, const_cast<uchar*>(src.bits()), static_cast<size_t>(src.bytesPerLine()));
		return true;

	default:
		std::cout << "ConvertQImage2Mat() - QImage format not handled in switch:" << src.format() << std::endl;
		return false;
	}
}
