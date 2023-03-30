#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/all_layers.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

#ifdef OBJECTDETECTION_API
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

extern "C" OBJECTDETECTION_API void FaceDetection(cv::Mat & frame, OBJECTDETECTION_API cv::CascadeClassifier face_cascade, cv::CascadeClassifier eyes_cascade);
extern "C" OBJECTDETECTION_API void ObjectDetecion(cv::Mat & frame, cv::dnn::Net & model, const std::vector<std::string>&class_names);
