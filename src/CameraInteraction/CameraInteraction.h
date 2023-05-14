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
	SHOW_CONFIDENCE,
	BINARY_THRESHOLDING,
	ZERO_THRESHOLDING,
	ADAPTIVE_THRESHOLDING,
	HISTOGRAM_EQUALIZATION,
	DETECT_EDGES
};
class CAMERAINTERACTION_API FrameOptions {
private:
	short confidence = 0;
	short threshold = 0;
	short binaryThresholdingValue = 0;
	short zeroThresholdingValue = 0;
	short adaptiveThresholdingValue = 0;
	bool histogramEqualization = false;
	bool detectEdges = false;
	bool showFeatures = false;
	bool flipH = true; // on camera, it's true by default
	bool flipV = false;
	bool showConfidence = false;
public:
	revertable_options lastChangedOption;

	void setConfidence(const short& val);
	short getConfidence() const;

	void setThreshold(const short& val);
	short getThreshold() const;

	void setShowFeatures(const bool& val);
	bool getShowFeatures() const;

	void setFlipH(const bool& val);
	bool getFlipH() const;

	void setFlipV(const bool& val);
	bool getFlipV() const;

	void setShowConfidence(const bool& val);
	bool getShowConfidence() const;

	void setBinaryThresholdingValue(const short& val);
	short getBinaryThresholdingValue() const;

	void setZeroThresholdingValue(const short& val);
	short getZeroThresholdingValue() const;

	void setAdaptiveThresholdingValue(const short& val);
	short getAdaptiveThresholdingValue() const;

	void setHistogramEqualization(const bool& val);
	bool getHistogramEqualization() const;

	void setDetectEdges(const bool& val);
	bool getDetectEdges() const;
};

class CAMERAINTERACTION_API OptionsHistory {
private:
	std::deque<FrameOptions> undoStack;
	std::deque<FrameOptions> redoStack;
	FrameOptions currentStatus;
	short maxOptions = 10;
	revertable_options lastChangedOption;
public:
	void add(revertable_options prop, short value);
	bool canRedo();
	bool canUndo();
	void reset();
	void undo();
	void redo();
	FrameOptions* get();
	std::string lastChange();
};

void CAMERAINTERACTION_API binaryThresholding(cv::Mat& image, short threshold);
void CAMERAINTERACTION_API zeroThresholding(cv::Mat& image, short threshold);
void CAMERAINTERACTION_API adaptiveThresholding(cv::Mat& image, short threshold);

void CAMERAINTERACTION_API histogramEqualization(cv::Mat& image);

void CAMERAINTERACTION_API detectEdges(cv::Mat& image);

class CAMERAINTERACTION_API Timer {
private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;
	int* counter;
public:
	Timer(int& c);
	~Timer();
};