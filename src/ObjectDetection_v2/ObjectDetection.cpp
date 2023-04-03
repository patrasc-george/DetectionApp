#include "ObjectDetection.h"

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
	cv::Mat blob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(frame.cols, frame.rows), cv::Scalar(127.5, 127.5, 127.5), true, false);
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
