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