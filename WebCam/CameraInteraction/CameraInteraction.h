#pragma once
#include <opencv2/opencv.hpp>

#ifdef CAMERAINTERACTION_API
#define CAMERAINTERACTION_API __declspec(dllexport)
#else
#define CAMERAINTERACTION_API __declspec(dllimport)
#endif

extern "C" CAMERAINTERACTION_API void Resolutions(const cv::VideoCapture & cap, const cv::Mat & frame, const std::string & windowName);
extern "C" CAMERAINTERACTION_API void FPS(const cv::Mat & frame, uint16_t & num_frames, float& start_time, float& end_time, float& elapsed_time, float& fps);