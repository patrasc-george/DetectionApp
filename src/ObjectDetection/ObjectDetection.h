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
enum type {
	cascade, network
};
class Detector {
protected:
	std::string modelPath;
	bool shouldSwapRB = false;
	type type;
public:
	virtual void detect(cv::Mat& image, bool = false) = 0;
	virtual void setMinConfidence(float c) {};
	virtual bool canDetectEyes() { return false; };
	virtual bool canDetectSmiles() { return false; };
	virtual int getType() { return 0; };
	virtual cv::Rect getLastRect() = 0;
	std::string currentClassName = "";
	virtual int init() = 0;
	virtual ~Detector() { std::cout << "detector destroyed" << std::endl; }

};

class OBJECTDETECTION_API FaceDetector : public Detector {
private:
	std::string eyeClassifierPath;
	std::string smileClassifierPath;
	cv::CascadeClassifier faceClassifier;
	cv::CascadeClassifier eyeClassifier;
	cv::CascadeClassifier smileClassifier;
	std::vector<cv::Rect> facesInFrame;
	std::vector<cv::Rect> eyes;
	std::vector<cv::Rect> smiles;
	bool eyesClassifierLoaded;
	bool smileClassifierLoaded;
public:
	FaceDetector(detectorProperties& props, std::string eyeClassifierPath = "\0", std::string smileClassifierPath = "\0");
	void detect(cv::Mat& image, bool showFeatures = false);
	cv::Rect getLastRect();
	int init();
	bool canDetectEyes() { return eyesClassifierLoaded; };
	bool canDetectSmiles() { return smileClassifierLoaded; };
	int getType() { return type; };
};

class OBJECTDETECTION_API ObjectDetector : public Detector {
private:
	std::vector<std::string> classNames;
	std::string classNamesPath;
	std::string infGraphPath;
	std::string framework;
	cv::Scalar meanValues;
	cv::dnn::Net model;
	bool showConfidence;
	float minConfidence = 0.6;
	cv::Rect lastRect;
public:
	ObjectDetector(detectorProperties props);
	void detect(cv::Mat& image , bool showConf = false);
	void setMinConfidence(float c);
	cv::Rect getLastRect();
	int init();
	int getType() { return type; };
};