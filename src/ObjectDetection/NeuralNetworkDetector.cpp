#include "NeuralNetworkDetector.h"
#include "interfaces.h"
#include "RectangleShape.h"
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

std::vector<std::unique_ptr<Shape>> NeuralNetworkDetector::detect(const cv::Mat& image) {
	// Convert the image to grayscale
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	// Apply a Gaussian filter for denoising
	cv::Mat denoised;
	cv::GaussianBlur(gray, denoised, cv::Size(5, 5), 0);

	// Enhance the image contrast
	cv::Mat enhanced;
	cv::equalizeHist(denoised, enhanced);

	// Resize the image to increase speed
	cv::Mat resized;
	cv::resize(enhanced, resized, cv::Size(), 0.5, 0.5);

	// Convert resized image to BGR for compatibility
	cv::Mat BGR;
	cv::cvtColor(resized, BGR, cv::COLOR_BGRA2BGR);

	// Create a blob from the normalized image
	cv::Mat blob = cv::dnn::blobFromImage(BGR);

	net.setInput(blob);
	cv::Mat detections;
	std::vector<std::unique_ptr<Shape>> shapes;

	try {
		/*
		In Debug mode, the forward method requires a string argument representing the layer,
		otherwise it throws a fatal exception and the whole app crashes.
		In Release mode, it automatically chooses the right layer
		*/
#ifdef NDEBUG
		detections = net.forward();
#else
		detections = net.forward("layer");
#endif
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		std::exception ex("No valid layer was provided to model.forward(). This would happen if the application is run in Debug mode.");
		throw ex;
		return shapes;
	}


    for (int i = 0; i < detections.size[2]; ++i) {
        float confidence = detections.at<float>(i, 2);
        if (confidence > confidenceThreshold) {
            int classId = static_cast<int>(detections.at<float>(i, 1));
            int left = static_cast<int>(detections.at<float>(i, 3) * image.cols);
            int top = static_cast<int>(detections.at<float>(i, 4) * image.rows);
            int right = static_cast<int>(detections.at<float>(i, 5) * image.cols);
            int bottom = static_cast<int>(detections.at<float>(i, 6) * image.rows);

            cv::Rect rect(left, top, right - left, bottom - top);
            std::string name = "Class " + classNames[classId];

            shapes.push_back(std::make_unique<RectangleShape>(rect, name, confidence));
        }
    }

    return shapes;
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
   
	cv::FileNode disabledClassesNode = fs["disabledClasses"];
	if (!disabledClassesNode.isSeq()) {
		return;
	}

	std::vector<std::string> disabledClassNames;
	fs["disabledClassNames"] >> disabledClassNames;
	for (const auto& className : disabledClassNames) {
		objectEnabledMap[className] = false;
	}
}
