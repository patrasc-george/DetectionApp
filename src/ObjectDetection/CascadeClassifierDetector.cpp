#include "CascadeClassifierDetector.h"
#include "RectangleShape.h"

CascadeClassifierDetector::CascadeClassifierDetector(const std::string& cascadeFilePath, const std::string& objectLabel)
    : objectLabel(objectLabel), cascadeFilePath(cascadeFilePath), enabled(true) {
    cascade.load(cascadeFilePath);
}

CascadeClassifierDetector::CascadeClassifierDetector()
    : enabled(false) {}

std::vector<std::unique_ptr<Shape>> CascadeClassifierDetector::detect(const cv::Mat& image) {
    std::vector<cv::Rect> detections;
    cascade.detectMultiScale(image, detections);

    std::vector<std::unique_ptr<Shape>> shapes;
    for (const cv::Rect& rect : detections) {
        shapes.push_back(std::make_unique<RectangleShape>(rect, objectLabel, 0.0)); // Zero, wew will not shouldRender it
    }

    return shapes;
}

void CascadeClassifierDetector::setEnabled(bool enable) {
    enabled = enable;
}

bool CascadeClassifierDetector::isEnabled() const {
    return enabled;
}

std::string CascadeClassifierDetector::getObjectLabel() const {
    return objectLabel;
}

void CascadeClassifierDetector::serialize(const std::string& filename) const {
    cv::FileStorage fs(filename, cv::FileStorage::WRITE);
    if (!fs.isOpened()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }
    fs << "detectorType" << "CASCADE";
    fs << "objectLabel" << objectLabel;
    fs << "cascadeFilePath" << cascadeFilePath;
}

void CascadeClassifierDetector::deserialize(const std::string& filename) {
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if (!fs.isOpened()) {
        throw std::runtime_error("Failed to open file for reading: " + filename);
    }

    fs["objectLabel"] >> objectLabel;
    fs["cascadeFilePath"] >> cascadeFilePath;
}

void CascadeClassifierDetector::read(cv::FileNode& node, CascadeClassifierDetector& detector) {
    if (node["objectLabel"].empty() || node["cascadeFilePath"].empty()) {
        throw std::runtime_error("Invalid or missing data in serialized file");
    }

    node["objectLabel"] >> detector.objectLabel;
    node["cascadeFilePath"] >> detector.cascadeFilePath;
}

std::string CascadeClassifierDetector::getCascadeFilePath() const {
    return cascadeFilePath;
}
