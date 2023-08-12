#include "RectangleShape.h"

RectangleShape::RectangleShape(const cv::Rect& rect, const std::string& label, double confidence)
    : rect(rect), label(label), confidence(confidence), renderEnabled(true), showConfidence(true),
    shapeColor(cv::Scalar(0, 255, 0)) {} // Default color: Green

void RectangleShape::render(cv::Mat& image) const {
    if (shouldRender()) {
        cv::rectangle(image, rect, shapeColor, 2);
        if (showConfidence) {
            cv::putText(image, label + ": " + std::to_string(confidence), cv::Point(rect.x, rect.y - 5),
                cv::FONT_HERSHEY_COMPLEX, 0.5, shapeColor, 1);
        }
    }
}

std::string RectangleShape::getLabel() const {
    return label;
}

double RectangleShape::getConfidence() const {
    return confidence;
}

void RectangleShape::setRenderStatus(bool render) {
    this->renderEnabled = render;
}

bool RectangleShape::shouldRender() const {
    return renderEnabled;
}

void RectangleShape::setConfidenceVisibility(bool visible) {
    showConfidence = visible;
}

void RectangleShape::setColor(const cv::Scalar& color) {
    shapeColor = color;
}
