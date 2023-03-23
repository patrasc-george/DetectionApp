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
/// <param name="newWidth">The desired width</param>
/// <param name="newHeight">The desired height</param>
/// <param name="inter">The interpolation method used to resize the image</param>
inline void cropOnResize(Mat& image, float newWidth, float newHeight, InterpolationFlags inter) {
    
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

inline void annotate(Mat &frame, String text, Point pos) {
    putText(frame, text, pos, 0, 0.5, Scalar(52, 235, 116), 1.75);
}

inline void drawLabel(Mat& input_image, string label, int left, int top)
{
    // Display the label at the top of the bounding box.
    int baseLine;
    Size label_size = getTextSize(label, 0, 0.7, 1, &baseLine);

    top = max(top, label_size.height);
    // Top left corner.
    Point tlc = Point(left, top);
    // Bottom right corner.
    Point brc = Point(left + label_size.width, top + label_size.height + baseLine);
    // Draw white rectangle.
    rectangle(input_image, tlc, brc, Scalar(0,0,0), FILLED);
    // Put the label on the black rectangle.
    putText(input_image, label, Point(left, top + label_size.height), 0, 0.7, Scalar(0, 255, 255), 1);
}

inline void detectFace(Mat& image) {
    Mat grayscale_image;
    cvtColor(image, grayscale_image, COLOR_BGR2GRAY);
    equalizeHist(grayscale_image, grayscale_image);

    CascadeClassifier cs("data\\haarcascade_frontalface_alt.xml");
    vector<Rect> objects;

    cs.detectMultiScale(grayscale_image, objects, 1.1, 2,
        0, Size(50, 50));

    if (objects.size() != 0) {
        for (auto&& obj : objects) {
            rectangle(image, obj, Scalar(52, 235, 116));
            drawLabel(image, "Face", obj.x, obj.y);
        }
    }

}