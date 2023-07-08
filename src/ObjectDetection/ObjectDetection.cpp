#include "ObjectDetection.h"

void drawLabel(cv::Mat& image, std::string label, int left, int top) {
    int baseLine;
    cv::Size label_size = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.7, 1.5, &baseLine);

    top = std::max(top, label_size.height);
    cv::Point tlc = cv::Point(left + 4, top + label_size.height + 6);
    putText(image, label, tlc, cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(143, 237, 76), 2);
}