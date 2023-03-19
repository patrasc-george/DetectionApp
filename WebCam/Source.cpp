#include "opencv2/opencv.hpp"
#include <iostream>

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

int main()
{
	cv::VideoCapture cap(0); // 0 - default camera
	cv::Mat frame;
	std::string windowName = "WebCam";
	namedWindow(windowName, cv::WINDOW_NORMAL);
	uint16_t num_frames = 0;
	float start_time, end_time, elapsed_time, fps;
	start_time = cv::getTickCount();

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
		cv::imshow(windowName, frame);
		if (cv::waitKey(30) >= 0)
			break;
	}
	return 0;
}