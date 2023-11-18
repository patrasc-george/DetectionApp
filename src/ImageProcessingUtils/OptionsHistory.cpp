#include "OptionsHistory.h"

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
	case TRUNC_THRESHOLDING:
		currentStatus.setTruncThresholdingValue(value);
		break;
	case ADAPTIVE_THRESHOLDING:
		currentStatus.setAdaptiveThresholdingValue(value);
		break;
	case GRAYSCALE_HISTOGRAM_EQUALIZATION:
		currentStatus.setGrayscaleHistogramEqualization(value);
		break;
	case COLOR_HISTOGRAM_EQUALIZATION:
		currentStatus.setColorHistogramEqualization(value);
		break;
	case SOBEL:
		currentStatus.setSobel(value);
		break;
	case TRIANGLE_THRESHOLDING:
		currentStatus.setTriangleThresholding(value);
		break;
	case BINOMIAL:
		currentStatus.setBinomial(value);
		break;
	case CANNY:
		currentStatus.setCanny(value);
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
	case TRUNC_THRESHOLDING:
		return "Truncate Thresholding";
	case ADAPTIVE_THRESHOLDING:
		return "adaptive thresholding";
	case GRAYSCALE_HISTOGRAM_EQUALIZATION:
		return "grayscale histogram equalization";
	case COLOR_HISTOGRAM_EQUALIZATION:
		return "histogram equalization";
	case SOBEL:
		return "sobel";
	case TRIANGLE_THRESHOLDING:
		return "triangle thresholding";
	case BINOMIAL:
		return "binomial";
	case CANNY:
		return "canny";
	default:
		return "last action";
	}
}