#pragma once
#include "interfaces.h"
#include <opencv2/opencv.hpp>

class CascadeClassifierDetector : public Detector, Serializable {
public:
    CascadeClassifierDetector(const std::string& cascadeFilePath, const std::string& objectLabel);
    CascadeClassifierDetector();
    std::vector<std::unique_ptr<Shape>> detect(const cv::Mat& image) override;

    void setEnabled(bool enable);
    bool isEnabled() const;
    std::string getObjectLabel() const;

    void serialize(const std::string& filename) const override;
    void deserialize(const std::string& filename) override;

    static void read(cv::FileNode& node, CascadeClassifierDetector& detector);

    std::string getCascadeFilePath() const;

private:
    std::string cascadeFilePath;
    cv::CascadeClassifier cascade;
    std::string objectLabel;
    bool enabled;
};
