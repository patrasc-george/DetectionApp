#pragma once
#include <opencv2/opencv.hpp>

#ifdef CAMERAINTERACTION_EXPORTS
#define CAMERAINTERACTION_API __declspec(dllexport)
#else
#define CAMERAINTERACTION_API __declspec(dllimport)
#endif

// unused
void cropOnResize(cv::Mat & image, int newWidth, int newHeight, cv::InterpolationFlags inter);

void CAMERAINTERACTION_API displayInfo(cv::Mat &image, bool showRes, bool showFps, int fps);

struct CAMERAINTERACTION_API Timer {
private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;
	int* counter;
public:
	Timer(int& c) {
		start = std::chrono::steady_clock::now();
		counter = &c;
	}
	~Timer() {
		end = std::chrono::steady_clock::now();
		duration = end - start;
		*counter = 1 / duration.count();
	}
};