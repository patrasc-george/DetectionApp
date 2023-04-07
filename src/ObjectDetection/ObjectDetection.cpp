#include "ObjectDetection.h"

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

void drawLabel(cv::Mat & image, std::string label, int left, int top) {
    int baseLine;
    cv::Size label_size = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.7, 1, &baseLine);

    top = std::max(top, label_size.height);
    cv::Point tlc = cv::Point(left, top);
    cv::Point brc = cv::Point(left + label_size.width, top + label_size.height + baseLine);
    rectangle(image, tlc, brc, cv::Scalar(0, 0, 0), cv::FILLED);
    putText(image, label, cv::Point(left, top + label_size.height), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 255), 1);
}

FaceDetector::FaceDetector(detectorProperties& props) {
    modelPath = props.modelPath;
    shouldSwapRB = props.shouldSwapRB;

    try {
        cs.load(modelPath);
}
    catch (const std::exception&) {
        std::cerr << "Couldn't load classifier!" << std::endl;
    }
}

void FaceDetector::detect(cv::Mat& image)
{
    cs.detectMultiScale(image, facesInFrame, 2, 2,
        0, cv::Size(50, 50));
    if (shouldDrawRect && facesInFrame.size() != 0) {
        for (auto&& obj : facesInFrame) {
            rectangle(image, obj, cv::Scalar(52, 235, 116));
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
    std::cout << "constructed\n";
}

void ObjectDetector::detect(cv::Mat& image) {
    cv::Mat blob = cv::dnn::blobFromImage(image, 1.0, cv::Size(300, 300), meanValues, shouldSwapRB, false);

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
            cv::rectangle(image, cv::Point(box_x, box_y), cv::Point(box_x + box_width, box_y + box_height), cv::Scalar(255, 255, 255), 2);
            drawLabel(image, classNames[classId - 1].c_str(), box_x, box_y);
        }
    }
}

Detector* Detector::initFaceDetector(detectorProperties& props) {
    return new FaceDetector(props);
}
Detector* Detector::initObjectDetector(detectorProperties& props) {
    return new ObjectDetector(props);
}