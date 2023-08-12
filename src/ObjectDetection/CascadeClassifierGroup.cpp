#include "CascadeClassifierGroup.h"

CascadeClassifierGroup::CascadeClassifierGroup() {}

void CascadeClassifierGroup::addDetector(const std::string& cascadeFilePath, const std::string& objectLabel) {
    detectors.emplace_back(cascadeFilePath, objectLabel);
    labelToIndexMap[objectLabel] = detectors.size() - 1;
}

std::vector<std::unique_ptr<Shape>> CascadeClassifierGroup::detect(const cv::Mat& image) {
    std::vector<std::unique_ptr<Shape>> allShapes;

    for (auto& detector : detectors) {
        if (isDetectorEnabled(detector.getObjectLabel())) {
            auto shapes = detector.detect(image);
            allShapes.insert(allShapes.end(), std::make_move_iterator(shapes.begin()), std::make_move_iterator(shapes.end()));
        }
    }

    return allShapes;
}

void CascadeClassifierGroup::enableDetector(const std::string& objectLabel, bool enable) {
    auto it = labelToIndexMap.find(objectLabel);
    if (it != labelToIndexMap.end()) {
        size_t detectorIndex = it->second;
        detectors[detectorIndex].setEnabled(enable);
    }
}

bool CascadeClassifierGroup::isDetectorEnabled(const std::string& objectLabel) const {
    auto it = labelToIndexMap.find(objectLabel);
    if (it != labelToIndexMap.end()) {
        size_t detectorIndex = it->second;
        return detectors[detectorIndex].isEnabled();
    }
    return false;
}

void CascadeClassifierGroup::serialize(const std::string& filename) const {
    cv::FileStorage fs(filename, cv::FileStorage::WRITE);
    if (!fs.isOpened()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    fs << "classifiers" << "[";
    for (const auto& detector : detectors) {
        fs << "{";
        fs << "objectLabel" << detector.getObjectLabel();
        fs << "enabled" << detector.isEnabled();
        fs << "cascadeFilePath" << detector.getCascadeFilePath();
        fs << "}";
    }
    fs << "]";
}

void CascadeClassifierGroup::deserialize(const std::string& filename) {
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if (!fs.isOpened()) {
        throw std::runtime_error("Failed to open file for reading: " + filename);
    }

    cv::FileNode detectorsNode = fs["classifiers"];
    if (!detectorsNode.isSeq()) {
        throw std::runtime_error("Invalid or missing detectors data in serialized file");
    }

    detectors.clear();
    for (cv::FileNode detectorNode : detectorsNode) {
        CascadeClassifierDetector detector;
        CascadeClassifierDetector::read(detectorNode, detector);
        detectors.push_back(detector);
    }
}

void CascadeClassifierGroup::read(cv::FileNode& node, CascadeClassifierGroup& group) {
    if (node["objectLabel"].empty() || node["cascadeFilePath"].empty()) {
        throw std::runtime_error("Invalid or missing data in serialized file");
    }

    std::string objectLabel;
    bool enabled;
    std::string cascadeFilePath;

    node["objectLabel"] >> objectLabel;
    node["enabled"] >> enabled;
    node["cascadeFilePath"] >> cascadeFilePath;

    group.addDetector(cascadeFilePath, objectLabel);
    group.enableDetector(objectLabel, enabled);
}
