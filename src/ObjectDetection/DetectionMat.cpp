#include "DetectionMat.h"

void DetectionMat::addShape(std::unique_ptr<Shape> shape) {
    shapes.push_back(std::move(shape));
}

void DetectionMat::sortShapesByConfidence() {
    std::sort(shapes.begin(), shapes.end(),
        [](const std::unique_ptr<Shape>& a, const std::unique_ptr<Shape>& b) {
            return a->getConfidence() > b->getConfidence();
        });
}

void DetectionMat::setShapeRenderStatus(size_t index, bool enableRender) {
    if (index < shapes.size()) {
        shapes[index]->setRenderStatus(enableRender);
    }
}

void DetectionMat::renderShapes(cv::Mat& image) {
    for (const auto& shape : shapes) {
        if (shape->shouldRender()) {
            shape->render(image);
        }
    }
}
