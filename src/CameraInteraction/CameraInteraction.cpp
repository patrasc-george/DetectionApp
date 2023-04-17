#include "CameraInteraction.h"

#include <iostream>
#include <opencv2/opencv.hpp>

void cropOnResize(cv::Mat& image, int newWidth, int newHeight, cv::InterpolationFlags inter) {

    float orgWidth = image.size().width;
    float orgHeight = image.size().height;

    float scale = newWidth / orgWidth;
    resize(image, image, cv::Size(), scale, scale, inter);

    if (image.size().height > newHeight) {
        image = image(cv::Range((image.size().height - newHeight) / 2, newHeight + (image.size().height - newHeight) / 2),
            cv::Range(0, image.size().width));
    }
    else {
        scale = newHeight / orgHeight;
        resize(image, image, cv::Size(), scale, scale, inter);
        image = image(cv::Range(0, image.size().height),
            cv::Range((image.size().width - newWidth) / 2, newWidth + (image.size().width - newWidth) / 2));
    }
}

void displayInfo(cv::Mat& image, bool showRes = true, bool showFps = false, int fps = 0) {
    short lines = 0;
    if (showRes) {
        lines++;
        putText(image, "Resolution: " + std::to_string(image.size().width) + "x" + std::to_string(image.size().height), cv::Point(10, 30*lines), cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(160, 189, 61));
    }
    if (showFps) {
        lines++;
        putText(image, "FPS: " + std::to_string(fps),cv::Point(10, 30*lines), cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(160, 189, 61));
    }
}