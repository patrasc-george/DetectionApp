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
	if (net.empty()) {
		try {
			net = cv::dnn::readNet(modelFilePath, configFilePath);
		}
		catch (cv::Exception& e) {
			throw std::runtime_error("Couldn't load neural network using " + modelFilePath + " and " + configFilePath);
		}
	}
	// Convert the image to grayscale
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	//// Apply a Gaussian filter for denoising
	cv::Mat denoised;
	cv::GaussianBlur(gray, denoised, cv::Size(5, 5), 0);

	//// Enhance the image contrast
	cv::Mat enhanced;
	cv::equalizeHist(denoised, enhanced);

	//// Resize the image to increase speed
	cv::Mat resized;
	cv::resize(enhanced, resized, cv::Size(), 0.5, 0.5);

	//// Convert resized image to BGR for compatibility
	cv::Mat BGR;
	cv::cvtColor(resized, BGR, cv::COLOR_BGRA2BGR);

	// Create a blob from the normalized image
	cv::Mat blob = cv::dnn::blobFromImage(BGR);

	net.setInput(blob);
	try {
		/*
		In Debug mode, the forward method requires a string argument representing the layer,
		otherwise it throws a fatal exception and the whole app crashes.
		In Release mode, it automatically chooses the right layer
		*/
#ifdef NDEBUG
		blob = net.forward();
#else
		blob = model.forward("layer");
#endif
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		std::exception ex("No valid layer was provided to model.forward(). This would happen if the application is run in Debug mode.");
		throw ex;
		return DetectionMat();
	}

	cv::Mat detectionMat = cv::Mat(blob.size[2], blob.size[3], CV_32F, blob.ptr<float>());
	DetectionMat det;

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
			det.add(shape);
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
