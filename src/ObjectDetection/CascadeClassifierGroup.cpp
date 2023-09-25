#include "CascadeClassifierGroup.h"

CascadeClassifierGroup::CascadeClassifierGroup() {}

void CascadeClassifierGroup::addDetector(const std::string& cascadeFilePath, const std::string& objectLabel) {
    detectors.emplace_back(std::make_unique<CascadeClassifierDetector>(cascadeFilePath, objectLabel));
    classifierStateMap[objectLabel].first = true;
    classifierStateMap[objectLabel].second = Detection::Shape::Rectangle;
}

DetectionMat CascadeClassifierGroup::detect(const cv::Mat& image) { 
    cv::Mat ROI;
    DetectionMat mat;
    int index = 0;

    mat.add(primaryDetector->detect(image));

    DetectionMat primary(mat);

    for (auto& primaryDetection : primary) {
        for (size_t i = 0; i < detectors.size(); ++i) {
            if (!isObjectEnabled(detectors.at(i)->getObjectLabel()))
                continue;

            ROI = image(primaryDetection.getRect());
            DetectionMat temp = (detectors.at(i)->detect(ROI));

            for (auto& det : temp) {
                cv::Rect rect = det.getRect();
                rect.x += primaryDetection.getRect().x;
                rect.y += primaryDetection.getRect().y;
                det.setRect(rect);
            }
            mat.add(temp);
        }
    }
    for (auto& det : mat) {
        det.shape = objectShape(det.getLabel());
    }

    return mat;
}

void CascadeClassifierGroup::setPrimary(const std::string& label) {
    for (auto it = detectors.begin(); it != detectors.end(); ++it) {
        if ((*it)->getObjectLabel() == label) {
            primaryDetector = std::move(*it);
            detectors.erase(it);
            enableObject(label, true);
            return;
        }
    }
    setPrimary(detectors.at(0)->getObjectLabel());
}

std::string CascadeClassifierGroup::getPrimary() const {
    return primaryDetector->getObjectLabel();
}

void CascadeClassifierGroup::enableObject(const std::string& objectLabel, bool enable) {
    auto it = classifierStateMap.find(objectLabel);
    if (it != classifierStateMap.end()) {
        it->second.first = enable;
    }
}

bool CascadeClassifierGroup::isObjectEnabled(const std::string& objectLabel) const {
    auto it = classifierStateMap.find(objectLabel);
    if (it != classifierStateMap.end()) {
        return it->second.first;
    }
    return false;
}

void CascadeClassifierGroup::setObjectShape(const std::string& objectLabel, Detection::Shape shape) {
    auto it = classifierStateMap.find(objectLabel);
    if (it != classifierStateMap.end()) {
        it->second.second = shape;
    }
}

Detection::Shape CascadeClassifierGroup::objectShape(const std::string& objectLabel) const {
    auto it = classifierStateMap.find(objectLabel);
    if (it != classifierStateMap.end()) {
        return it->second.second;
    }
    return Detection::Shape::Rectangle;
}

void CascadeClassifierGroup::serialize(const std::string& filename) const {
    cv::FileStorage fs(filename, cv::FileStorage::WRITE);
    if (!fs.isOpened()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    fs << "type" << "CASCADE_GROUP";
    fs << "classifiers" << "[";

    fs << "{";
    fs << "objectLabel" << primaryDetector->getObjectLabel();
    fs << "enabled" << true;
    fs << "shape" << objectShape(primaryDetector->getObjectLabel());
    fs << "cascadeFilePath" << primaryDetector->getCascadeFilePath();
    fs << "}";

    for (const auto& detector : detectors) {
        fs << "{";
        fs << "objectLabel" << detector->getObjectLabel();
        fs << "enabled" << isObjectEnabled(detector->getObjectLabel());
        fs << "shape" << objectShape(detector->getObjectLabel());
        fs << "cascadeFilePath" << detector->getCascadeFilePath();
        fs << "}";
    }
    fs << "]";
    fs << "primary" << getPrimary();
    fs.release();
}

void CascadeClassifierGroup::deserialize(const std::string& filename) {
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if (!fs.isOpened()) {
        throw std::runtime_error("Failed to open file for reading: " + filename);
    }

    std::string primary;
    fs["primary"] >> primary;

    cv::FileNode detectorsNode = fs["classifiers"];
    if (!detectorsNode.isSeq()) {
        throw std::runtime_error("Invalid or missing detectors data in serialized file");
    }

    int index = 0;
    detectors.clear();
    for (const cv::FileNode& node : detectorsNode) {
        if (node["cascadeFilePath"].empty()) {
            throw std::runtime_error("Invalid or missing data in serialized file");
        }

        std::string objectLabel;
        bool enabled;
        Detection::Shape shape;
        std::string cascadeFilePath;

        if (node["objectLabel"].empty())
            objectLabel = std::string("Object ") + std::to_string(index);
        else
            node["objectLabel"] >> objectLabel;

        if (node["enabled"].empty())
            enabled = true;
        else
            node["enabled"] >> enabled;

        if (node["shape"].empty())
            shape = Detection::Shape::Rectangle;
        else
            node["shape"] >> shape;

        node["cascadeFilePath"] >> cascadeFilePath;

        addDetector(cascadeFilePath, objectLabel);
        enableObject(objectLabel, enabled);
        setObjectShape(objectLabel, shape);
    }
    if (primary.empty())
        primary = detectors.at(0)->getObjectLabel();
    setPrimary(primary);

    serializationFilePath = filename;
}

std::string CascadeClassifierGroup::getSerializationFile() const {
    return serializationFilePath;
}

std::vector<std::string> CascadeClassifierGroup::getObjectLabels() const {
    std::vector<std::string> labels;
   
    for (auto it = classifierStateMap.begin(); it != classifierStateMap.end(); ++it) {
        labels.push_back(it->first);
    }

    return labels;
}

CanToggleObjects* CascadeClassifierGroup::toObjectToggler() {
    return dynamic_cast<CanToggleObjects*>(this);
}
