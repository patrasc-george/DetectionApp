#pragma once
#include <opencv2/opencv.hpp>

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif


void drawLabel(cv::Mat & image, std::string label, int left, int top);

struct detectorProperties {
	std::string modelPath;
	std::string classNamesPath;
	std::string infGraphPath;
	std::string framework;
	bool shouldSwapRB;
	cv::Scalar meanValues;
	detectorProperties()
		: modelPath("\0"), classNamesPath("\0"), infGraphPath("\0"), framework("\0"), shouldSwapRB(true), meanValues(cv::Scalar(0,0,0)) {}
};

struct foundObject {
	std::string label;
	cv::Rect boundingBox;
};

class Detector {
protected:
	std::string modelPath;
	bool shouldSwapRB = false;
	bool active = false;
	bool shouldDrawRect = true;

public:
	static Detector* initFaceDetector(detectorProperties&);
	static Detector* initObjectDetector(detectorProperties&);
	virtual void detect(cv::Mat& image, bool detectEyes = false) = 0;
};

class OBJECTDETECTION_API FaceDetector : public Detector {
private:
	std::string eyeClassifierPath;
	cv::CascadeClassifier faceClassifier;
	cv::CascadeClassifier eyeClassifier;
	std::vector<cv::Rect> facesInFrame;
public:
	FaceDetector(detectorProperties& props, std::string eyeClassifierPath = "\0");
	void detect(cv::Mat& image, bool v2 = false);
};

class OBJECTDETECTION_API ObjectDetector : public Detector {
private:
	std::vector<std::string> classNames;
	std::string classNamesPath;
	std::string infGraphPath;
	std::string framework;
	cv::Scalar meanValues;
	cv::dnn::Net model;
public:
	ObjectDetector(detectorProperties props);
	void detect(cv::Mat& image , bool detectEyes = false);
};