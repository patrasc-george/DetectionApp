#include "ObjectDetection.h"

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
using namespace cv;

void drawLabel(Mat & image, std::string label, int left, int top) {
    int baseLine;
    Size label_size = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.7, 1, &baseLine);

    top = max(top, label_size.height);
    Point tlc = Point(left, top);
    Point brc = Point(left + label_size.width, top + label_size.height + baseLine);
    rectangle(image, tlc, brc, Scalar(0, 0, 0), FILLED);
    putText(image, label, Point(left, top + label_size.height), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 255), 1);
}

FaceDetector::FaceDetector(detectorProperties props) {
    modelPath = props.modelPath;
    shouldSwapRB = props.shouldSwapRB;

    try {
        cs.load(modelPath);
    }
    catch (const std::exception&) {
        std::cerr << "Couldn't load classifier!" << std::endl;
    }
}

void FaceDetector::detect(Mat& image)
{
    cs.detectMultiScale(image, facesInFrame, 1.75, 2,
        0, Size(50, 50));
    if (shouldDrawRect && facesInFrame.size() != 0) {
        for (auto&& obj : facesInFrame) {
            rectangle(image, obj, Scalar(52, 235, 116));
            drawLabel(image, "Face", obj.x, obj.y);
        }
    }
}

ObjectDetector::ObjectDetector(detectorProperties props) {
    modelPath = props.modelPath;
    classNamesPath = props.classNamesPath;
    configPath = props.configPath;
    framework = props.framework;
    shouldSwapRB = props.shouldSwapRB;
    meanValues = props.meanValues;

    try {
        std::ifstream ifs(classNamesPath);
        std::string line;
        while (getline(ifs, line))
        {
            classNames.push_back(line);
        }
    }
    catch (const std::exception&) {
        std::cerr << "Couldn't load class names!" << std::endl;
    }
    try {
        model = cv::dnn::readNet(modelPath, framework);
    }
    catch (const std::exception&) {
        std::cerr << "Couldn't load detection model!" << std::endl;
    }
}

void ObjectDetector::detect(Mat& image) {
    cv::Mat blob = cv::dnn::blobFromImage(image, 1.0, Size(300, 300), meanValues,
        shouldSwapRB, false);

    model.setInput(blob);
    cv::Mat output = model.forward();

    cv::Mat detectionMat(output.size[2], output.size[3], CV_32F, output.ptr<float>());

    for (int i = 0; i < detectionMat.rows; i++) {
        int classId = detectionMat.at<float>(i, 1);
        float confidence = detectionMat.at<float>(i, 2);

        if (confidence > 0.4 && shouldDrawRect) {
            int box_x = (int) (detectionMat.at<float>(i, 3) * image.cols);
            int box_y = (int) (detectionMat.at<float>(i, 4) * image.rows);
            int box_width = (int) (detectionMat.at<float>(i, 5) * image.cols - box_x);
            int box_height = (int) (detectionMat.at<float>(i, 6) * image.rows - box_y);
            cv::rectangle(image, Point(box_x, box_y), Point(box_x + box_width, box_y + box_height), Scalar(255, 255, 255), 2);
            drawLabel(image, classNames[classId - 1].c_str(), box_x, box_y);
        }
    }
}
