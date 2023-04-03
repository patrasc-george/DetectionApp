#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/all_layers.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "../CameraInteraction_v2/CameraInteraction.h"
#include "../ObjectDetection_v2/ObjectDetection.h"

uint16_t Choice()
{
	uint16_t choice;
	std::cout << "\n\n\n\n\n\n";
	std::cout << "-------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "1. Face Detection" << std::endl;
	std::cout << "2. Object Detection" << std::endl;
	std::cout << "3. None" << std::endl;
	std::cout << "Choice: ";
	std::cin >> choice;
	std::cout << "-------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "\n\n\n\n\n\n";
	return choice;
}

void LoadModel(cv::CascadeClassifier& face_cascade, cv::CascadeClassifier& eyes_cascade, std::vector<std::string>& class_names, cv::dnn::Net& model)
{
	// Face detection load
	face_cascade.load(std::string(getenv("OpenCV_DIR")) + "\\..\\sources\\data\\haarcascades\\haarcascade_frontalface_default.xml");
	eyes_cascade.load(std::string(getenv("OpenCV_DIR")) + "\\..\\sources\\data\\haarcascades\\haarcascade_eye.xml");

	/*
	// Object detection load
	std::ifstream ifs(std::string("input/object_detection_classes_coco.txt").c_str());
	std::string line;

	while (getline(ifs, line))
		class_names.push_back(line);

	model = cv::dnn::readNet("../../WebCam/WebCam/input/frozen_inference_graph.pb",
		"../../WebCam/WebCam/input/ssd_mobilenet_v2_coco_2018_03_29.pbtxt.txt",
		"TensorFlow");
	*/
}

int main()
{
	float start_time, end_time, elapsed_time, fps;
	uint16_t num_frames = 0;
	cv::VideoCapture cap(0); // 0 - default camera
	cv::Mat frame;
	std::string windowName = "WebCam";
	namedWindow(windowName, cv::WINDOW_NORMAL);
	start_time = cv::getTickCount();
	std::vector<std::string> class_names;
	cv::CascadeClassifier face_cascade;
	cv::CascadeClassifier eyes_cascade;
	cv::dnn::Net model;
	uint16_t choice = 1;
	LoadModel(face_cascade, eyes_cascade, class_names, model);

	if (!cap.isOpened())
	{
		std::cout << "Camera not opened!" << std::endl;
		return -1;
	}

	while (true)
	{
		if (!cap.read(frame)) {
			std::cout << "Unable to read frame!" << std::endl;
			break;
		}
		Resolutions(cap, frame, windowName);
		FPS(frame, num_frames, start_time, end_time, elapsed_time, fps);
		FaceDetection(frame, face_cascade, eyes_cascade);
		cv::imshow(windowName, frame);
		if (cv::waitKey(30) >= 0)
			break;
	}
	return 0;
}