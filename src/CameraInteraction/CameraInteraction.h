#pragma once
#include <opencv2/opencv.hpp>

#ifdef CAMERAINTERACTION_EXPORTS
#define CAMERAINTERACTION_API __declspec(dllexport)
#else
#define CAMERAINTERACTION_API __declspec(dllimport)
#endif


void CAMERAINTERACTION_API displayInfo(cv::Mat &image, std::string key, std::string value, cv::Point pos);
void CAMERAINTERACTION_API binaryThresholding(cv::Mat& inputImage, int threshold);
void CAMERAINTERACTION_API histogramEqualization(cv::Mat& inputImage);

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