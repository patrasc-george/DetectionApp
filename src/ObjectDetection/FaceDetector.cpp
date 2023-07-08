#include "FaceDetector.h"

FaceDetector::FaceDetector(detectorProperties& props, std::string eyeClassifierPath, std::string smileClassifierPath) {
	currentClassName = "face";
	modelPath = props.modelPath;
	shouldSwapRB = props.shouldSwapRB;
	this->eyeClassifierPath = eyeClassifierPath;
	this->smileClassifierPath = smileClassifierPath;
	type = cascade;
}

int FaceDetector::init() {
	if (modelPath == "\0")
		return -2;
	if (!faceClassifier.load(modelPath))
		return -3;
	if (!eyeClassifierPath.empty())
		eyesClassifierLoaded = eyeClassifier.load(eyeClassifierPath);
	if (!smileClassifierPath.empty())
		smileClassifierLoaded = smileClassifier.load(smileClassifierPath);

	return 1;
}

void FaceDetector::detect(cv::Mat& image, bool showFeatures) {
	if (image.type() == CV_8UC4)
		cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);
	cv::Mat gray;
	if (image.type() != CV_8UC1)
		cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	else
		gray = image;

	faceClassifier.detectMultiScale(gray, facesInFrame, 1.1, 6);
	if (facesInFrame.size() == 0)
		return;
	for (auto&& face : facesInFrame) {
		if (showFeatures == false)
			break;
		// find eyes on face
		if (eyesClassifierLoaded) {
			cv::Mat faceROI = gray(face);
			faceROI = faceROI(cv::Range(0, face.height / 2), cv::Range(0, face.width));
			eyeClassifier.detectMultiScale(faceROI, eyes);
		}
		// find smile on face
		if (smileClassifierLoaded) {
			cv::Mat faceROI = gray(face);
			faceROI = faceROI(cv::Range(face.height / 2, face.height), cv::Range(0, face.width));
			smileClassifier.detectMultiScale(faceROI, smiles, 1.2, 6);
		}
	}
	for (auto&& face : facesInFrame) {
		if (face.size().width > 10 || face.size().height > 10) {
			rectangle(image, face, cv::Scalar(143, 237, 76), 2);
			drawLabel(image, currentClassName, face.x, face.y);
		}
		if (showFeatures == false)
			break;
		for (auto&& eye : eyes) {
			cv::Point eye_center(face.x + eye.x + eye.width / 2, face.y + eye.y + eye.height / 2);
			short radius = cvRound((eye.width + eye.height) * 0.25);
			circle(image, eye_center, radius, cv::Scalar(239, 190, 98), 2);
		}
		for (auto&& smile : smiles) {
			rectangle(image, cv::Rect(face.x + smile.x, face.y + face.width - smile.y * 2, smile.width, smile.height), cv::Scalar(100, 167, 255), 2);
		}
	}
}

cv::Rect FaceDetector::getLastRect() {
	return facesInFrame.empty() ? cv::Rect() : facesInFrame.back();
}