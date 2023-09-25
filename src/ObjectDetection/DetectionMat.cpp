#include "DetectionMat.h"

void DetectionMat::add(std::shared_ptr<Detection>& detection) {
    detections.push_back(detection);
}

void DetectionMat::add(const DetectionMat& other) {
    for (auto& det : other.getAll()) {
        detections.push_back(std::make_shared<Detection>(det));
    }
}

void DetectionMat::sortByConfidence() {
    std::sort(detections.begin(), detections.end(),
        [](const std::shared_ptr<Detection>& a, const std::shared_ptr<Detection>& b) {
            return a->getConfidence() > b->getConfidence();
        });
}

void DetectionMat::setShapeRenderStatus(size_t index, bool enableRender) {
    if (index < detections.size()) {
        detections[index]->setRenderStatus(enableRender);
    }
}

void DetectionMat::setShowConfidence(bool show) {
    for (auto& detection : detections)
        detection.get()->setConfidenceVisibility(show);
}

void DetectionMat::render(cv::Mat& image) {
    for (const auto& detection : detections)
        if (detection->shouldRender()) {
            detection->render(image);
        }
}

std::vector<Detection> DetectionMat::getAll() const
{
    std::vector<Detection> vec;

    for (const auto& det : detections)
        vec.push_back(*det);

    return vec;
}

DetectionMat::DetectionMat(const std::vector<std::shared_ptr<Detection>>& detections)
    : detections(detections)
{
}



DetectionMat::iterator::iterator(std::vector<std::shared_ptr<Detection>>::iterator it) : iter(it) {}

DetectionMat::iterator& DetectionMat::iterator::operator++() {
    ++iter;
    return *this;
}

DetectionMat::iterator DetectionMat::iterator::operator++(int) {
    iterator temp = *this;
    ++iter;
    return temp;
}

Detection& DetectionMat::iterator::operator*() {
    return *(*iter);
}

bool DetectionMat::iterator::operator==(const iterator& other) const {
    return iter == other.iter;
}

bool DetectionMat::iterator::operator!=(const iterator& other) const {
    return iter != other.iter;
}

DetectionMat::iterator DetectionMat::begin() {
    return iterator(detections.begin());
}

DetectionMat::iterator DetectionMat::end() {
    return iterator(detections.end());
}

bool DetectionMat::empty() {
    return detections.empty();
}