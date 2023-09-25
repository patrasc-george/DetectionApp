#include "NeuralNetworkDetector.h"
#include "Detection.h"
#include <fstream>

NeuralNetworkDetector::NeuralNetworkDetector(const std::string& modelFilePath, const std::string& configFilePath, const std::string& classesFilePath)
	: modelFilePath(modelFilePath)
	, configFilePath(configFilePath)
	, classesFilePath(classesFilePath)
	, confidenceThreshold(0.5)
{
	try {
		net = cv::dnn::readNet(modelFilePath, configFilePath);
	}
	catch (cv::Exception& e) {
		throw std::runtime_error("Couldn't load neural network using \"" + modelFilePath + "\" and \"" + "\"" + configFilePath + "\":\n" + e.what());
	}
	loadClasses(this->classesFilePath);
}

NeuralNetworkDetector::NeuralNetworkDetector() : confidenceThreshold(0.5) 
{
}

DetectionMat NeuralNetworkDetector::detect(const cv::Mat& image) {

	if (net.empty()) {
		try {
			net = cv::dnn::readNet(modelFilePath, configFilePath);
		}
		catch (cv::Exception& e) {
			throw std::runtime_error("Couldn't load neural network using " + modelFilePath + " and " + configFilePath);
		}
	}
	cv::Mat blob;
	cv::Mat copy = image;

	// Convert the image to grayscale
	cv::cvtColor(copy, copy, cv::COLOR_BGR2GRAY);

	// Enhance the image contrast
	cv::equalizeHist(copy, copy);


	// Convert resized image to BGR for compatibility
	cv::cvtColor(copy, copy, cv::COLOR_BGRA2BGR);

	// Resize the image to increase speed
	cv::resize(copy, copy, cv::Size(), 0.5, 0.5, 6);

	cv::dnn::blobFromImage(copy, blob);

	net.setInput(blob);
	try {
		std::vector<cv::Mat> outputs;
		net.forward(outputs);
		blob = outputs[0];
	}
	catch (const std::exception& e) {
		std::exception ex(e.what());
		return DetectionMat();
	}

	std::vector<cv::Rect> boxes;
	std::vector<float> confidences;
	std::vector<int> classes;


	cv::Mat detectionMat = cv::Mat(blob.size[2], blob.size[3], CV_32F, blob.ptr<float>());

	for (int i = 0; i < detectionMat.rows; i++) {
		int classId = detectionMat.at<float>(i, 1);
		float confidence = detectionMat.at<float>(i, 2);

		if (confidence > confidenceThreshold) {
			int box_x = (int)(detectionMat.at<float>(i, 3) * image.cols);
			int box_y = (int)(detectionMat.at<float>(i, 4) * image.rows);
			int box_width = (int)(detectionMat.at<float>(i, 5) * image.cols - box_x);
			int box_height = (int)(detectionMat.at<float>(i, 6) * image.rows - box_y);
			cv::Rect rect(box_x, box_y, box_width, box_height);

			boxes.push_back(rect);
			classes.push_back(classId - 1);
			confidences.push_back(confidence);
		}
	}

	float nmsThreshold = 0.4;
	std::vector<int> indices;

	cv::dnn::NMSBoxes(boxes, confidences, confidenceThreshold, nmsThreshold, indices);

	DetectionMat det;
	for (const auto& index : indices)
	{
		cv::Rect rect = boxes[index];
		float confidence = confidences[index];
		int classId = classes[index];

		std::string c = classNames[classId];
		std::shared_ptr<Detection> shape = std::make_shared<Detection>(rect, c, confidence);
		shape.get()->setRenderStatus(objectEnabledMap[c]);
		det.add(shape);
	}

	return det;
}

void NeuralNetworkDetector::adjustThreshold(float newThreshold) {
	if (newThreshold >= 0 && newThreshold <= 1)
		confidenceThreshold = newThreshold;
}

float NeuralNetworkDetector::getCurrentThreshold() {
	return confidenceThreshold;
}

void NeuralNetworkDetector::enableObject(const std::string& label, bool enable) {
	for (auto& obj : objectEnabledMap) {
		if (obj.first == label) {
			obj.second = enable;
			break;
		}
	}
}

bool NeuralNetworkDetector::isObjectEnabled(const std::string& name) const {
	for (auto& obj : objectEnabledMap) {
		if (obj.first == name) {
			return obj.second;
		}
	}
	return false;
}

void NeuralNetworkDetector::serialize(const std::string& filename) const {
	cv::FileStorage fs(filename, cv::FileStorage::WRITE);
	if (!fs.isOpened()) {
		throw std::runtime_error("Failed to open file for writing: " + filename);
	}

	fs << "type" << "NETWORK";
	fs << "modelFilePath" << modelFilePath;
	if (!configFilePath.empty())
		fs << "configFilePath" << configFilePath;
	fs << "labelsFilePath" << classesFilePath;

	std::vector<std::string> disabledClassNames;
	for (const auto& classEntry : objectEnabledMap) {
		if (!classEntry.second) {
			disabledClassNames.push_back(classEntry.first);
		}
	}
	fs << "disabledClassNames" << disabledClassNames;
	fs.release();
}

void NeuralNetworkDetector::deserialize(const std::string& filename) {
	cv::FileStorage fs(filename, cv::FileStorage::READ);
	if (!fs.isOpened()) {
		throw std::runtime_error("Failed to open file for reading: " + filename);
	}

	fs["modelFilePath"] >> modelFilePath;
	fs["configFilePath"] >> configFilePath;
	fs["labelsFilePath"] >> classesFilePath;

	loadClasses(classesFilePath);

	std::vector<std::string> disabledClassNames;
	fs["disabledClassNames"] >> disabledClassNames;
	for (const auto& className : disabledClassNames) {
		objectEnabledMap[className] = false;
	}

	fs.release();
	serializationFile = filename;
}

std::string NeuralNetworkDetector::getSerializationFile() const {
	return serializationFile;
}

std::vector<std::string> NeuralNetworkDetector::getObjectLabels() const {
	return classNames;
}

ThresholdAdjuster* NeuralNetworkDetector::toThresholdAdjuster() {
	return dynamic_cast<ThresholdAdjuster*>(this);
}
CanToggleObjects* NeuralNetworkDetector::toObjectToggler() {
	return dynamic_cast<CanToggleObjects*>(this);
}

void NeuralNetworkDetector::loadClasses(const std::string& filePath) {
	classesFilePath = filePath;
	std::ifstream labelsStream(classesFilePath);
	if (!labelsStream.is_open()) {
		throw std::runtime_error("Failed to open labels file: " + classesFilePath);
	}

	std::string className;
	while (std::getline(labelsStream, className)) {
		classNames.push_back(className);
		objectEnabledMap.insert({ className, true });
	}
}
