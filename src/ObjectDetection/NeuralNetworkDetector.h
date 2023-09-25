#pragma once
#include "Detector.h"
#include "ThresholdAdjuster.h"
#include "CanToggleObjects.h"

#include <opencv2/opencv.hpp>

class OBJECTDETECTION_API NeuralNetworkDetector : public Detector, ThresholdAdjuster, CanToggleObjects {
public:
    NeuralNetworkDetector(const std::string& modelFilePath, const std::string& configFilePath, const std::string& classesFilePath);
    NeuralNetworkDetector();
    DetectionMat detect(const cv::Mat& image) override;
    void adjustThreshold(float newThreshold) override;
    float getCurrentThreshold() override;

    void loadClasses(const std::string& classesFilePath);

    void enableObject(const std::string& label, bool enable) override;
    bool isObjectEnabled(const std::string& label) const override;

    void serialize(const std::string& filename) const override;
    void deserialize(const std::string& filename) override;
    std::string getSerializationFile() const override;

    std::vector<std::string> getObjectLabels() const override;

    ThresholdAdjuster* toThresholdAdjuster() override;
    CanToggleObjects* toObjectToggler() override;
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
