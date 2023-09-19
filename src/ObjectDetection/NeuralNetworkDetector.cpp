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
	catch (cv::Exception&) {
		throw std::runtime_error("Couldn't load neural network using " + modelFilePath + " and " + configFilePath);
	}

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

NeuralNetworkDetector::NeuralNetworkDetector() : confidenceThreshold(0.5)
{
}

DetectionMat NeuralNetworkDetector::detect(const cv::Mat& image) {
	bool isONNX = false;
	if (configFilePath.find(".onnx") != std::string::npos)
		isONNX = true;

	if (net.empty()) {
		try {
			if (isONNX)
				net = cv::dnn::readNet(configFilePath);
			else
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

	// Create a blob from the normalized image
	if (isONNX)
	{
		// Resize the image to increase speed
		cv::resize(copy, copy, cv::Size(224, 224), 6);
		cv::dnn::blobFromImage(copy, blob, 1. / 255., cv::Size(224, 224));
	}
	else
	{
		cv::resize(copy, copy, cv::Size(), 0.5, 0.5, 6);
		cv::dnn::blobFromImage(copy, blob);
	}
	net.setInput(blob);
	try {
		/*
		In Debug mode, the forward method requires a string argument representing the layer,
		otherwise it throws a fatal exception and the whole app crashes.
		In Release mode, it automatically chooses the right layer
		*/
#ifdef NDEBUG
		std::vector<cv::Mat> outputs;
		net.forward(outputs);
		blob = outputs[0];
#else
		blob = net.forward("layer");
#endif
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		std::exception ex("No valid layer was provided to model.forward(). This would happen if the application is run in Debug mode.");
		throw ex;
		return DetectionMat();
	}

	DetectionMat det;
	if (isONNX)
	{
		float* data = (float*)blob.data;
		const int rows = blob.size[1];

		float scaleX = image.cols / (float)copy.cols;
		float scaleY = image.rows / (float)copy.rows;
		std::vector<cv::Rect> boxes;
		std::vector<float> confidences;
		std::vector<int> classes;

		for (int i = 0; i < rows; i++) {
			float confidence = data[4];
			if (confidence > confidenceThreshold)
			{
				int classId = 0;
				float maxConfidence = 0;
				for (int j = 0; j < classNames.size(); j++)
					if (data[5 + j] > maxConfidence)
					{
						classId = j;
						maxConfidence = data[5 + j];
					}
				int box_width = int(data[2] * scaleX);
				int box_height = int(data[3] * scaleY);
				int box_x = int(data[0] * scaleX - 0.5 * box_width);
				int box_y = int(data[1] * scaleY - 0.5 * box_height);
				cv::Rect rect(box_x, box_y, box_width, box_height);
				boxes.push_back(rect);
				confidences.push_back(confidence);
				classes.push_back(classId);
			}
			data += 5 + classNames.size();
		}
		float nmsThreshold = 0.4;
		std::vector<int> indices;

		cv::dnn::NMSBoxes(boxes, confidences, confidenceThreshold, nmsThreshold, indices);

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
	}
	else
	{
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

				std::string c = classNames[classId - 1];
				std::shared_ptr<Detection> shape = std::make_shared<Detection>(rect, c, confidence);
				shape.get()->setRenderStatus(objectEnabledMap[c]);
				det.add(shape);
			}
		}
	}

	return det;
}

void NeuralNetworkDetector::adjustThreshold(float newThreshold) {
	if (newThreshold >= 0 && newThreshold <= 1)
		confidenceThreshold = newThreshold;
}

void NeuralNetworkDetector::enableObject(const std::string& label, bool enable) {
	for (auto& obj : objectEnabledMap) {
		if (obj.first == label) {
			obj.second = enable;
			break;
		}
	}
}

void NeuralNetworkDetector::serialize(const std::string& filename) const {
	cv::FileStorage fs(filename, cv::FileStorage::WRITE);
	if (!fs.isOpened()) {
		throw std::runtime_error("Failed to open file for writing: " + filename);
	}

	fs << "type" << "NETWORK";
	fs << "modelFilePath" << modelFilePath;
	fs << "configFilePath" << configFilePath;
	fs << "labelsFilePath" << classesFilePath;
	//fs << "confidenceThreshold" << confidenceThreshold;
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
	fs["labelsFilePath"] >> classesFilePath; // Read labels file path
	//fs["confidenceThreshold"] >> confidenceThreshold;

	std::ifstream labelsStream(classesFilePath);
	if (!labelsStream.is_open()) {
		throw std::runtime_error("Failed to open labels file: " + classesFilePath);
	}

	std::string className;
	while (std::getline(labelsStream, className)) {
		classNames.push_back(className);
		objectEnabledMap.insert({ className, true });
	}

	std::vector<std::string> disabledClassNames;
	fs["disabledClassNames"] >> disabledClassNames;
	for (const auto& className : disabledClassNames) {
		objectEnabledMap[className] = false;
	}
	fs.release();
}

std::string NeuralNetworkDetector::getSerializationFile() const
{
	return serializationFile;
}

std::vector<std::string> NeuralNetworkDetector::getClassNames()
{
	return classNames;
}
