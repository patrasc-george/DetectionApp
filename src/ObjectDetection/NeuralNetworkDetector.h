#pragma once
#include "Detector.h"
#include "ThresholdAdjuster.h"
#include <opencv2/opencv.hpp>

class OBJECTDETECTION_API NeuralNetworkDetector : public Detector, ThresholdAdjuster {
public:
    NeuralNetworkDetector(const std::string& modelFilePath, const std::string& configFilePath, const std::string& classesFilePath);
    NeuralNetworkDetector();
    DetectionMat detect(const cv::Mat& image) override;
    void adjustThreshold(float newThreshold) override;
    float getCurrentThreshold() override;
    void loadClasses(const std::string& classesFilePath);
    void enableObject(const std::string& label, bool enable);
    bool isObjectEnabled(const std::string& name);

    void serialize(const std::string& filename) const override;
    void deserialize(const std::string& filename) override;
    std::string getSerializationFile() const override;

    std::vector<std::string> getClassNames();

protected:
    cv::dnn::Net net;
    std::vector<std::string> classNames;
    float confidenceThreshold;
    std::unordered_map<std::string, bool> objectEnabledMap;

    std::string modelFilePath;
    std::string classesFilePath;

    std::string serializationFile;

private:
    std::string configFilePath;
};
