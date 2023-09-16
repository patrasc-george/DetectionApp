#include "ObjectDetector.h"

#include <algorithm>

cv::Rect ObjectDetector::getLastRect() {
	return lastRect;
}

ObjectDetector::ObjectDetector(detectorProperties props) {
	modelPath = props.modelPath;
	classNamesPath = props.classNamesPath;
	infGraphPath = props.infGraphPath;
	framework = props.framework;
	shouldSwapRB = props.shouldSwapRB;
	meanValues = props.meanValues;
	type = network;
}

int ObjectDetector::init() {
	if (modelPath == "\0")
		return -2;
	if (infGraphPath == "\0")
		return -4;
	std::ifstream ifs(classNamesPath);
	std::string line;
	while (getline(ifs, line))
		classNames.push_back(std::make_pair(line, false));
	try {
		model = cv::dnn::readNet(infGraphPath, modelPath, framework);
	}
	catch (cv::Exception&) {
		return -5;
	}
	return 1;
}

void ObjectDetector::setClassNamesValues(const std::vector<bool> classesToShow)
{
	for (int i = 0; i < classNames.size(); i++)
		classNames[i].second = classesToShow.at(i);
}

void ObjectDetector::sort()
{
	sortedClassNames.clear();
	std::set<std::string> detectClasses = getDetectedClassNames();
	std::set<std::string> undetectedClasses;

	for (const auto& className : detectClasses)
		sortedClassNames.push_back(className);

	for (const auto& className : classNames)
		if (std::find(sortedClassNames.begin(), sortedClassNames.end(), className.first) == sortedClassNames.end())
			undetectedClasses.insert(className.first);

	for (const auto& className : undetectedClasses)
		sortedClassNames.push_back(className);
}

std::vector<std::string> ObjectDetector::getSortedClassNames() const
{
	return sortedClassNames;
}

std::set<std::string> ObjectDetector::getDetectedClassNames() const
{
	std::set<std::string> detectedClasses;
	for (int i = 0; i < detectionMat.rows; i++)
	{
		int classId = detectionMat.at<float>(i, 1) - 1;
		detectedClasses.insert(classNames[classId].first);
	}
	return detectedClasses;
}

void ObjectDetector::detect(cv::Mat& image, bool showConf) {
	cv::Mat copy = image;

	// Convert the image to grayscale
	cv::cvtColor(copy, copy, cv::COLOR_BGR2GRAY);

	// Enhance the image contrast
	cv::equalizeHist(copy, copy);

	// Resize the image to increase speed
	cv::resize(copy, copy, cv::Size(), 0.5, 0.5, 6);

	// Convert resized image to BGR for compatibility
	cv::cvtColor(copy, copy, cv::COLOR_BGRA2BGR);

	// Create a blob from the normalized image
	cv::Mat blob = cv::dnn::blobFromImage(copy);

	std::vector<std::string> layers = model.getLayerNames();

	model.setInput(blob);
	try {
		/*
		In Debug mode, the forward method requires a string argument representing the layer,
		otherwise it throws a fatal exception and the whole app crashes.
		In Release mode, it automatically chooses the right layer
		*/
#ifdef NDEBUG
		blob = model.forward();
#else
		blob = model.forward("layer");
#endif
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		std::exception ex("No valid layer was provided to model.forward(). This would happen if the application is run in Debug mode.");
		throw ex;
		return;
	}

	detectionMat = cv::Mat(blob.size[2], blob.size[3], CV_32F, blob.ptr<float>());

	for (int i = 0; i < detectionMat.rows; i++) {
		int classId = detectionMat.at<float>(i, 1);
		float confidence = detectionMat.at<float>(i, 2);

		if (confidence > minConfidence && classNames[classId - 1].second) {
			int box_x = (int)(detectionMat.at<float>(i, 3) * image.cols);
			int box_y = (int)(detectionMat.at<float>(i, 4) * image.rows);
			int box_width = (int)(detectionMat.at<float>(i, 5) * image.cols - box_x);
			int box_height = (int)(detectionMat.at<float>(i, 6) * image.rows - box_y);
			lastRect = cv::Rect(box_x, box_y, box_width, box_height);

			currentClassName = classNames[classId - 1].first.c_str();
			std::stringstream ss;
			ss << currentClassName;
			if (showConf)
				ss << ": confidence = " + std::to_string((int)(confidence * 100)) + "%";
			drawLabel(image, ss.str(), lastRect);
		}
	}
	cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);
}

void ObjectDetector::setMinConfidence(float c) {
	if (c > 0 && c < 1)
		minConfidence = c;
}
