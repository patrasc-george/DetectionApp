#pragma once

#include "interfaces.h"
#include <opencv2/core.hpp>

class RectangleShape : public Shape {
public:
    RectangleShape(const cv::Rect& rect, const std::string& label, double confidence);

    void render(cv::Mat& image) const override;
    std::string getLabel() const override;
    double getConfidence() const override;
    void setRenderStatus(bool render) override;
    bool shouldRender() const override;
    void setConfidenceVisibility(bool visible);
    void setColor(const cv::Scalar& color);

private:
    cv::Rect rect;
    std::string label;
    double confidence;
    bool renderEnabled;
    bool showConfidence;
    cv::Scalar shapeColor;
};