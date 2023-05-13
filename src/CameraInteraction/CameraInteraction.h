#pragma once
#include <opencv2/opencv.hpp>
#include <stack>

#ifdef CAMERAINTERACTION_EXPORTS
#define CAMERAINTERACTION_API __declspec(dllexport)
#else
#define CAMERAINTERACTION_API __declspec(dllimport)
#endif

static enum CAMERAINTERACTION_API revertable_options {
	CONFIDENCE,
	THRESHOLD,
	SHOW_FEATURES,
	FLIP_HORIZONTAL,
	FLIP_VERTICAL,
	SHOW_CONFIDENCE
};
class FrameOptions {
private:
	short confidence = 0;
	short threshold = 0;
	bool showFeatures = false;
	bool flipH = true; // on camera, it's true by default
	bool flipV = false;
	bool showConfidence = false;
public:
	void setConfidence(const short& val) { confidence = val; }
	short getConfidence() const { return confidence; }

	void setThreshold(const short& val) { threshold = val; }
	short getThreshold() const { return threshold; }

	void setShowFeatures(const bool& val) { showFeatures = val; }
	bool getShowFeatures() const { return showFeatures; }

	void setFlipH(const bool& val) { flipH = val; }
	bool getFlipH() const { return flipH; }

	void setFlipV(const bool& val) { flipV = val; }
	bool getFlipV() const { return flipV; }

	void setShowConfidence(const bool& val) { showConfidence = val; }
	bool getShowConfidence() const { return showConfidence; }
};

class OptionsStack {
private:
	std::stack<FrameOptions> undoStack;
	std::stack<FrameOptions> redoStack;
	FrameOptions currentStatus;
	// if we want to limit the size of the stack we need access to its bottom, we should consider using a different data type, such as a deque
	//short maxOptions = 10; 
public:
	void addToStack(revertable_options prop, short value) {
		undoStack.push(currentStatus);
		switch (prop) {
		case FLIP_HORIZONTAL:
			currentStatus.setFlipH(value);
			break;
		case FLIP_VERTICAL:
			currentStatus.setFlipV(value);
			break;
		default:
			return;
		}
		if (!redoStack.empty())
			redoStack = std::stack<FrameOptions>();
	}
	bool canRedo() {
		return !redoStack.empty();
	}
	bool canUndo() {
		return !undoStack.empty();
	}
	void reset() {
		undoStack = std::stack<FrameOptions>();
		redoStack = std::stack<FrameOptions>();
	}
	void undo() {
		if (!undoStack.empty()) {
			redoStack.push(currentStatus);
			currentStatus = undoStack.top();
			undoStack.pop();
		}
	}
	void redo() {
		if (!redoStack.empty()) {
			undoStack.push(currentStatus);
			currentStatus = redoStack.top();
			redoStack.pop();
		}
	}
	FrameOptions* get() {
		return &currentStatus;
	}
};

void CAMERAINTERACTION_API binaryThresholding(cv::Mat& image, short threshold);
void CAMERAINTERACTION_API zeroThresholding(cv::Mat& image, short threshold);
void CAMERAINTERACTION_API adaptiveThresholding(cv::Mat& image, short threshold);

void CAMERAINTERACTION_API histogramEqualization(cv::Mat& image);

void CAMERAINTERACTION_API detectEdges(cv::Mat& image);

struct CAMERAINTERACTION_API Timer {
private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;
	int* counter;
public:
	Timer(int& c) {
		start = std::chrono::steady_clock::now();
		duration = start - start;
		counter = &c;
	}
	~Timer() {
		end = std::chrono::steady_clock::now();
		duration = end - start;
		*counter = 1 / duration.count();
	}
};