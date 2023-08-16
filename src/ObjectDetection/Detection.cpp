#include <opencv2/imgproc.hpp>

#include "Detection.h"

Detection::Detection(const cv::Rect& rect, const std::string& label, double confidence)
    : rect(rect), label(label), confidence(confidence), renderEnabled(true), showConfidence(true),
    shapeColor(cv::Scalar(0, 255, 0)) {} // Default color: Green

void Detection::render(cv::Mat& image) const {
    if (shouldRender()) {
        cv::rectangle(image, rect, shapeColor, 2);
        if (showConfidence) {
            cv::putText(image, label + ": " + std::to_string(confidence), cv::Point(rect.x, rect.y - 5),
                cv::FONT_HERSHEY_COMPLEX, 0.5, shapeColor, 1);
        }
    }
}

std::string Detection::getLabel() const {
    return label;
}

double Detection::getConfidence() const {
    return confidence;
}

void Detection::setRenderStatus(bool render) {
    this->renderEnabled = render;
}

bool Detection::shouldRender() const {
    return renderEnabled;
}

void Detection::setConfidenceVisibility(bool visible) {
    showConfidence = visible;
}

void Detection::setColor(const cv::Scalar& color) {
    shapeColor = color;
}
