#include "pch.h"
#include "CameraInteraction.h"

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;

void cropOnResize(Mat& image, int newWidth, int newHeight, InterpolationFlags inter) {

    float orgWidth = image.size().width;
    float orgHeight = image.size().height;

    float scale = newWidth / orgWidth;
    resize(image, image, Size(), scale, scale, inter);

    if (image.size().height > newHeight) {
        image = image(Range((image.size().height - newHeight) / 2, newHeight + (image.size().height - newHeight) / 2),
            Range(0, image.size().width));
    }
    else {
        scale = newHeight / orgHeight;
        resize(image, image, Size(), scale, scale, inter);
        image = image(Range(0, image.size().height),
            Range((image.size().width - newWidth) / 2, newWidth + (image.size().width - newWidth) / 2));
    }
}

void calculateFps(float& fps, std::chrono::time_point<std::chrono::system_clock>& start_time) {
    std::chrono::time_point<std::chrono::system_clock> end_time = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    fps = 1.0 / elapsed_seconds.count();
    start_time = end_time;
}

void displayInfo(Mat& image, Size nativeRes, double fps) {
    putText(image, "Native Resolution: " + std::to_string(nativeRes.width) + "x" + std::to_string(nativeRes.height), Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);
    putText(image, "Resolution: " + std::to_string(image.cols) + "x" + std::to_string(image.rows), Point(10, 60), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);
    putText(image, "FPS: " + std::to_string(fps), Point(10, 90), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);
}