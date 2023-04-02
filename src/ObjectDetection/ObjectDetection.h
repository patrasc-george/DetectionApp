#pragma once
#include <opencv2/opencv.hpp>

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

extern OBJECTDETECTION_API void drawLabel(cv::Mat & image, std::string label, int left, int top);

extern OBJECTDETECTION_API void detectObjects(cv::Mat & image, cv::CascadeClassifier & cs, std::vector<cv::Rect>&objects);

struct detectorProperties {
	std::string modelPath = "\0";
	std::string classNamesPath = "\0";
	std::string configPath = "\0";
	std::string framework = "\0";
	bool shouldSwapRB = false;
};

struct foundObject {
	std::string label;
	cv::Rect boundingBox;
};

extern OBJECTDETECTION_API class Detector {
protected:
	std::string modelPath;
	bool shouldSwapRB = false;
	bool active = false;
	bool shouldDrawRect = true;
	std::vector<cv::Rect> objectsInFrame;

public:
	virtual void detect(cv::Mat& image) = 0;
};

extern OBJECTDETECTION_API class FaceDetector : public Detector {
private:
	cv::CascadeClassifier cs;
public:
	FaceDetector(detectorProperties props);
	void detect(cv::Mat& image);

};

extern OBJECTDETECTION_API class ObjectDetector : public Detector {
private:
	std::string classNamesPath;
	std::string configPath;
	std::string framework;
	std::vector<std::string> classNames;
public:
	ObjectDetector(detectorProperties props);
	void detect(cv::Mat& image);
};