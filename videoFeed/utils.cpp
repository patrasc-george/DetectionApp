#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

/// <summary>
/// It resizes the image to a desired a size, while keeping the aspect-ratio.
/// In order to do so, the function wil crop the portion of the image that overflows the "container"
/// (given by the desired size) and keep the view on the center of the image.
/// </summary>
/// <param name="image">The Mat object to be modified</param>
/// <param name="windowSize.width">The desired width</param>
/// <param name="windowSize.height">The desired height</param>
/// <param name="inter">The interpolation method used to resize the image</param>
inline void cropOnResize(Mat& image, int newWidth, int newHeight, InterpolationFlags inter) {

    float orgWidth = image.size().width;
    float orgHeight = image.size().height;

    Mat newImg;
    float scale = newWidth / orgWidth;
    resize(image, newImg, Size(), scale, scale, inter);

    if (newImg.size().height > newHeight) {
        newImg = newImg(Range((newImg.size().height - newHeight) / 2, newHeight + (newImg.size().height - newHeight) / 2),
            Range(0, newImg.size().width));
    }
    else {
        scale = newHeight / orgHeight;
        resize(image, newImg, Size(), scale, scale, inter);
        newImg = newImg(Range(0, newImg.size().height),
            Range((newImg.size().width - newWidth) / 2, newWidth + (newImg.size().width - newWidth) / 2));
    }

    image = newImg;
}



inline void calculateFps(float& fps, chrono::time_point<chrono::system_clock>& start_time) {
    chrono::time_point<chrono::system_clock> end_time = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end_time - start_time;
    fps = 1.0 / elapsed_seconds.count();
    start_time = end_time;
}

inline void displayInfo(Mat& image, Size nativeRes, double fps) {
    // Add text to image
    putText(image, "Native Resolution: " + to_string(nativeRes.width) + "x" + to_string(nativeRes.height), Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);
    putText(image, "Resolution: " + to_string(image.cols) + "x" + to_string(image.rows), Point(10, 60), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);
    putText(image, "FPS: " + to_string(fps), Point(10, 90), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);
}

inline void drawLabel(Mat& image, string label, int left, int top)
{
    int baseLine;
    Size label_size = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.7, 1, &baseLine);

    top = max(top, label_size.height);
    Point tlc = Point(left, top);
    Point brc = Point(left + label_size.width, top + label_size.height + baseLine);
    rectangle(image, tlc, brc, Scalar(0, 0, 0), FILLED);
    putText(image, label, Point(left, top + label_size.height), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 255), 1);
}

inline void detectObjects(Mat& image, CascadeClassifier& cs, vector<Rect>& objects) {

    cs.detectMultiScale(image, objects, 1.75, 2,
        0, Size(50, 50));
    if (objects.size() != 0) {
        for (auto&& obj : objects) {
            rectangle(image, obj, Scalar(52, 235, 116));
            drawLabel(image, "Face", obj.x, obj.y);
        }
    }

}