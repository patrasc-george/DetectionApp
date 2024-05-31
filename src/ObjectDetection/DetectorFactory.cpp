#include <opencv2/core.hpp>

#include "DetectorFactory.h"
#include "CascadeClassifierDetector.h"
#include "CascadeClassifierGroup.h"
#include "NeuralNetworkDetector.h"
#include "OnnxDetector.h"

Detector* DetectorFactory::createDetectorFromFile(const std::string& filePath) {
	try {
		cv::FileStorage fs(filePath, cv::FileStorage::READ);

		if (!fs.isOpened()) {
			throw std::runtime_error("Failed to open file");
		}

		std::string detectorType;
		std::string model;
		fs["type"] >> detectorType;
		fs["modelFilePath"] >> model;
		fs.release();
		Detector* d = nullptr;

		if (detectorType == "CASCADE") {
			d = new CascadeClassifierDetector();
		}
		else if (detectorType == "NETWORK") {
			if (model.substr(model.find_last_of(".") + 1) == "onnx")
				d = new OnnxDetector();
			else
				d = new NeuralNetworkDetector();
		}
		else if (detectorType == "CASCADE_GROUP") {
			d = new CascadeClassifierGroup();
		}
		else {
			throw std::runtime_error("Unknown detector type");
		}
		d->deserialize(filePath);
		return d;

	}
	catch (const std::exception& e) {
		return nullptr;
	}
}
