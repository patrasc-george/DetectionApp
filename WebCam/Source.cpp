#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/all_layers.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

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
	face_cascade.load("C:/Users/George Patrasc/Downloads/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml");
	eyes_cascade.load("C:/Users/George Patrasc/Downloads/opencv/sources/data/haarcascades/haarcascade_eye.xml");

	// Object detection load
	std::ifstream ifs(std::string("C:/Users/George Patrasc/Desktop/dnn/input/object_detection_classes_coco.txt").c_str());
	std::string line;

	while (getline(ifs, line))
		class_names.push_back(line);

	model = cv::dnn::readNet("C:/Users/George Patrasc/Desktop/dnn/input/frozen_inference_graph.pb",
		"C:/Users/George Patrasc/Desktop/dnn/input/ssd_mobilenet_v2_coco_2018_03_29.pbtxt.txt",
		"TensorFlow");
}
void Resize(const cv::VideoCapture& cap, const cv::Mat& frame, const std::string& windowName)
{
	// Get the size of the window and of the captured image
	float windowWidth = cv::getWindowImageRect(windowName).width;
	float imgWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);

	// Calculate the scaling factor to resize the image
	float scale = windowWidth / imgWidth;

	cv::Mat resized_img;

	// Resize the image using the calculated scaling factor
	resize(frame, resized_img, cv::Size(), scale, scale, cv::INTER_AREA);
}

void Resolutions(const cv::VideoCapture& cap, const cv::Mat& frame, const std::string& windowName)
{
	// Get the size of the display window
	cv::Rect windowSize = cv::getWindowImageRect(windowName);

	// Display the camera resolution
	cv::putText(frame, "Camera: " + std::to_string(int(cap.get(cv::CAP_PROP_FRAME_WIDTH))) + "x"
		+ std::to_string(int(cap.get(cv::CAP_PROP_FRAME_HEIGHT))), cv::Point(10, 40),
		cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);

	// Display the window resolution on the video frame
	cv::putText(frame, "Window: " + std::to_string(windowSize.width) + "x"
		+ std::to_string(windowSize.height), cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
}

void FPS(const cv::Mat& frame, uint16_t& num_frames, float& start_time, float& end_time, float& elapsed_time, float& fps)
{
	num_frames++;
	end_time = cv::getTickCount();

	// Calculate the elapsed time since the last print
	elapsed_time = (end_time - start_time) / cv::getTickFrequency();

	// Check if more than 1 second has elapsed
	if (elapsed_time > 1) {
		fps = num_frames / elapsed_time;
		num_frames = 0;
		start_time = end_time;
	}
	cv::putText(frame, "FPS: " + std::to_string(fps), cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
}

void FaceDetection(cv::Mat& frame, cv::CascadeClassifier face_cascade, cv::CascadeClassifier eyes_cascade)
{
	// The color image is converted to grayscale
	cv::Mat frame_gray;
	cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(frame_gray, frame_gray);

	// Detect faces
	std::vector<cv::Rect> faces;
	face_cascade.detectMultiScale(frame_gray, faces);

	for (int i = 0; i < faces.size(); i++)
	{
		cv::Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(frame, center, cv::Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, cv::Scalar(255, 0, 255), 4);

		// The area of interest in the image is obtained
		cv::Mat faceROI = frame_gray(faces[i]);

		// In each face, detect eyes
		std::vector<cv::Rect> eyes;
		eyes_cascade.detectMultiScale(faceROI, eyes);

		for (int j = 0; j < eyes.size(); j++)
		{
			cv::Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
			int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
			circle(frame, eye_center, radius, cv::Scalar(255, 0, 0), 4);
		}
	}
}

void ObjectDetecion(cv::Mat& frame, cv::dnn::Net& model, const std::vector<std::string>& class_names)
{
	// Image processing
	cv::Mat blob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(300, 300), cv::Scalar(127.5, 127.5, 127.5), true, false);
	model.setInput(blob);
	cv::Mat detection = model.forward();

	for (int i = 0; i < detection.size[2]; i++)
	{
		float confidence = detection.at<float>(i, 2);

		if (confidence > 0.5)
		{
			int class_id = static_cast<int>(detection.at<float>(i, 1));
			int x_left_bottom = static_cast<int>(frame.cols * detection.at<float>(i, 3));
			int y_left_bottom = static_cast<int>(frame.rows * detection.at<float>(i, 4));
			int x_right_top = static_cast<int>(frame.cols * detection.at<float>(i, 5));
			int y_right_top = static_cast<int>(frame.rows * detection.at<float>(i, 6));

			rectangle(frame, cv::Point(x_left_bottom, y_left_bottom), cv::Point(x_right_top, y_right_top), cv::Scalar(0, 255, 0), 2);
		}
	}
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
	uint16_t choice = Choice();
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
		Resize(cap, frame, windowName);
		Resolutions(cap, frame, windowName);
		FPS(frame, num_frames, start_time, end_time, elapsed_time, fps);
		if (choice == 1)
			FaceDetection(frame, face_cascade, eyes_cascade);
		if (choice == 2)
			ObjectDetecion(frame, model, class_names);
		cv::imshow(windowName, frame);
		if (cv::waitKey(30) >= 0)
			break;
	}
	return 0;
}