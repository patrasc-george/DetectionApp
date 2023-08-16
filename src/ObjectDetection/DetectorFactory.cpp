#include <opencv2/core.hpp>

#include "DetectorFactory.h"
#include "CascadeClassifierDetector.h"
#include "CascadeClassifierGroup.h"
#include "NeuralNetworkDetector.h"

Detector* DetectorFactory::createDetectorFromFile(const std::string& filePath) {
    try {
        cv::FileStorage fs(filePath, cv::FileStorage::READ);

        if (!fs.isOpened()) {
            throw std::runtime_error("Failed to open file");
        }

        std::string detectorType;
        fs["type"] >> detectorType;
        fs.release();
        if (detectorType == "CASCADE") {
            CascadeClassifierDetector* d = new CascadeClassifierDetector();
            d->deserialize(filePath);
            return d;
        }
        else if (detectorType == "NETWORK") {
            NeuralNetworkDetector* d = new NeuralNetworkDetector();
            d->deserialize(filePath);
            return d;
        }
        else if (detectorType == "CASCADE_NETWORK") {
            CascadeClassifierGroup* d = new CascadeClassifierGroup();
            d->deserialize(filePath);
            return d;
        }
        else {
            throw std::runtime_error("Unknown detector type");
        }
    }
    catch (const std::exception& e) {
        return nullptr; // Return nullptr on failure
    }
}
