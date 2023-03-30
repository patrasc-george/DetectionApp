#include "pch.h"
#include "ObjectDetection.h"

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;

void drawLabel(Mat& image, std::string label, int left, int top) {
    int baseLine;
    Size label_size = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.7, 1, &baseLine);

    top = max(top, label_size.height);
    Point tlc = Point(left, top);
    Point brc = Point(left + label_size.width, top + label_size.height + baseLine);
    rectangle(image, tlc, brc, Scalar(0, 0, 0), FILLED);
    putText(image, label, Point(left, top + label_size.height), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 255), 1);
}

void detectObjects(Mat& image, CascadeClassifier& cs, std::vector<Rect>& objects) {

    cs.detectMultiScale(image, objects, 1.75, 2,
        0, Size(50, 50));
    if (objects.size() != 0) {
        for (auto&& obj : objects) {
            rectangle(image, obj, Scalar(52, 235, 116));
            drawLabel(image, "Face", obj.x, obj.y);
        }
    }


}