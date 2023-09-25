#include "CascadeClassifierDetector.h"
#include "Detection.h"
#include <opencv2/imgproc.hpp>

CascadeClassifierDetector::CascadeClassifierDetector(const std::string& cascadeFilePath, const std::string& objectLabel)
    : objectLabel(objectLabel), cascadeFilePath(cascadeFilePath) {
    cascade.load(cascadeFilePath);
}

CascadeClassifierDetector::CascadeClassifierDetector()
{
}

DetectionMat CascadeClassifierDetector::detect(const cv::Mat& image) {
    std::vector<cv::Rect> detections;
    if (cascade.empty())
        cascade.load(cascadeFilePath);

    if (image.type() == CV_8UC4)
        cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);
    cv::Mat gray;
    if (image.type() != CV_8UC1)
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    else
        gray = image;

    cascade.detectMultiScale(gray, detections, 1.1, 6);

    DetectionMat detMat;
    for (const cv::Rect& rect : detections) {
        if (rect.size().width > 10 || rect.size().height > 10) {
            std::shared_ptr<Detection> d = std::make_shared<Detection>(rect, objectLabel, 0.0); // Zero, we will not render it
            detMat.add(d);
        }
    }
    return detMat;
}

std::string CascadeClassifierDetector::getObjectLabel() const {
    return objectLabel;
}

void CascadeClassifierDetector::serialize(const std::string& filename) const {
    cv::FileStorage fs(filename, cv::FileStorage::WRITE);
    if (!fs.isOpened()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }
    fs << "type" << "CASCADE";
    fs << "objectLabel" << objectLabel;
    fs << "cascadeFilePath" << cascadeFilePath;
    fs.release();
}

void CascadeClassifierDetector::deserialize(const std::string& filename) {
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if (!fs.isOpened()) {
        throw std::runtime_error("Failed to open file for reading: " + filename);
    }

    fs["objectLabel"] >> objectLabel;
    fs["cascadeFilePath"] >> cascadeFilePath;

    serializationFilePath = filename;
    fs.release();
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

std::string CascadeClassifierDetector::getSerializationFile() const {
    return serializationFilePath;
}
