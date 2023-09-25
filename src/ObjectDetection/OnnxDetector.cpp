#include "OnnxDetector.h"

OnnxDetector::OnnxDetector(const std::string& modelFilePath, const std::string& classesFilePath) : NeuralNetworkDetector() {
	this->modelFilePath = modelFilePath;
	this->classesFilePath = classesFilePath;
	try {
		net = cv::dnn::readNetFromONNX(modelFilePath);
	}
	catch (cv::Exception& e) {
		throw std::runtime_error("Couldn't load neural network using \"" + modelFilePath + "\":\n" + e.what());
	}
	loadClasses(this->classesFilePath);
}

OnnxDetector::OnnxDetector() : NeuralNetworkDetector()
{
}

DetectionMat OnnxDetector::detect(const cv::Mat& image) {
	if (net.empty()) {
		try {
			net = cv::dnn::readNet(modelFilePath);
		}
		catch (cv::Exception& e) {
			throw std::runtime_error("Couldn't load neural network using \"" + modelFilePath + "\" and \"" + modelFilePath + "\":\n" + e.what());
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
	cv::resize(copy, copy, cv::Size(224, 224), 6);

	cv::dnn::blobFromImage(copy, blob, 1. / 255., cv::Size(224, 224));

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


