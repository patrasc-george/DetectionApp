#include "FrameOptions.h"

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

void FrameOptions::setTruncThresholdingValue(const short& val) {
	truncThresholdingValue = val;
}

short FrameOptions::getTruncThresholdingValue() const {
	return truncThresholdingValue;
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

void FrameOptions::setGrayscaleHistogramEqualization(const bool& val) {
	grayscaleHistogramEqualization = val;
}

bool FrameOptions::getGrayscaleHistogramEqualization() const {
	return grayscaleHistogramEqualization;
}

void FrameOptions::setColorHistogramEqualization(const bool& val) {
	colorHistogramEqualization = val;
}

bool FrameOptions::getColorHistogramEqualization() const {
	return colorHistogramEqualization;
}

void FrameOptions::setSobel(const bool& val) {
	sobel = val;
}

bool FrameOptions::getSobel() const {
	return sobel;
}

void FrameOptions::setTriangleThresholding(const bool& val)
{
	triangleThresholding = val;
}

bool FrameOptions::getTriangleThresholding() const
{
	return triangleThresholding;
}

void FrameOptions::setBinomial(const short& val)
{
	binomial = val;
}

short FrameOptions::getBinomial() const
{
	return binomial;
}

void FrameOptions::setCanny(const short& val)
{
	canny = val;
}
short FrameOptions::getCanny() const
{
	return canny;
}