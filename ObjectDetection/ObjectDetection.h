#pragma once
#include <opencv2/opencv.hpp>

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

extern "C" OBJECTDETECTION_API void drawLabel(cv::Mat & image, std::string label, int left, int top);

extern "C" OBJECTDETECTION_API void detectObjects(cv::Mat & image, cv::CascadeClassifier & cs, std::vector<cv::Rect>&objects);