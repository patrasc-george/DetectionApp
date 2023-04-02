#pragma once
#include <opencv2/opencv.hpp>

#ifdef CAMERAINTERACTION_EXPORTS
#define CAMERAINTERACTION_API __declspec(dllexport)
#else
#define CAMERAINTERACTION_API __declspec(dllimport)
#endif

extern "C" CAMERAINTERACTION_API void cropOnResize(cv::Mat & image, int newWidth, int newHeight, cv::InterpolationFlags inter);

extern "C" CAMERAINTERACTION_API void calculateFps(float& fps, std::chrono::time_point<std::chrono::system_clock>&start_time);

extern "C" CAMERAINTERACTION_API void displayInfo(cv::Mat & image, cv::Size nativeRes, double fps);