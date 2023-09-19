#pragma once
#include "NeuralNetworkDetector.h"

#include <opencv2/opencv.hpp>

class OBJECTDETECTION_API OnnxDetector : public NeuralNetworkDetector {
public:
    OnnxDetector(const std::string& modelFilePath, const std::string& classesFilePath);
    OnnxDetector();
    DetectionMat detect(const cv::Mat& image) override;
};
