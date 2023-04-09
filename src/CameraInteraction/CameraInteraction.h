#pragma once
#include <opencv2/opencv.hpp>

#ifdef CAMERAINTERACTION_EXPORTS
#define CAMERAINTERACTION_API __declspec(dllexport)
#else
#define CAMERAINTERACTION_API __declspec(dllimport)
#endif

// unused
void cropOnResize(cv::Mat & image, int newWidth, int newHeight, cv::InterpolationFlags inter);

void CAMERAINTERACTION_API displayInfo(cv::Mat & image, cv::Size nativeRes, float fps);

struct CAMERAINTERACTION_API Timer {
private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;
	float* counter;
public:
	Timer(float& c) {
		start = std::chrono::steady_clock::now();
		counter = &c;
	}
	~Timer() {
		end = std::chrono::steady_clock::now();
		duration = end - start;
		*counter = 1.0f / duration.count();
	}
};

// GEORGE
void CAMERAINTERACTION_API FPS(const cv::Mat & frame, uint16_t & num_frames, float& start_time, float& end_time, float& elapsed_time, float& fps);