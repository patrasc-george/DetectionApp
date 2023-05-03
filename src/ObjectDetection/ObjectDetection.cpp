#include "ObjectDetection.h"

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

void drawLabel(cv::Mat & image, std::string label, int left, int top) {
    int baseLine;
    cv::Size label_size = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.7, 1.5, &baseLine);

    //top = std::max(top, label_size.height);
    //cv::Point tlc = cv::Point(left, top);
    //cv::Point brc = cv::Point(left + label_size.width, top + label_size.height + baseLine);
    //rectangle(image, tlc, brc, cv::Scalar(147, 167, 255), cv::FILLED);
    putText(image, label, cv::Point(left, top - label_size.height + 1.5), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(147, 167, 255), 2);
}

FaceDetector::FaceDetector(detectorProperties& props, std::string eyeClassifierPath, std::string smileClassifierPath) {
    currentClassName = "face";
    modelPath = props.modelPath;
    shouldSwapRB = props.shouldSwapRB;
    this->eyeClassifierPath = eyeClassifierPath;
    this->smileClassifierPath = smileClassifierPath;
    type = cascade;
}
bool FaceDetector::init() {
    try {
        faceClassifier.load(modelPath);
    }
    catch (const std::exception& ex) {
        std::cerr << "Couldn't load face classifier for \"" << modelPath.substr(modelPath.find_last_of("\\"), modelPath.back()) + "\"" << std::endl;
        throw ex;
        return false;
    }

    try {
        eyeClassifier.load(eyeClassifierPath);
        eyesClassifierLoaded = true;
    }
    catch (const std::exception&) {
        std::cerr << "Couldn't load eye classifier for \"" << modelPath.substr(modelPath.find_last_of("/"), modelPath.back()) + "\"" << std::endl;
        eyesClassifierLoaded = false;
    }
    try {
        smileClassifier.load(smileClassifierPath);
        smileClassifierLoaded = true;
    }
    catch (const std::exception&) {
        std::cerr << "Couldn't load smile classifier for \"" << modelPath.substr(modelPath.find_last_of("/"), modelPath.back()) + "\"" << std::endl;
        smileClassifierLoaded = false;
    }
    return true;
}

void FaceDetector::detect(cv::Mat& image, bool showFeatures) {
    faceClassifier.detectMultiScale(image, facesInFrame, 1.4, 10);
    if (facesInFrame.size() == 0)
        return;
    for (auto&& face : facesInFrame) {
        if (showFeatures == false)
            break;
        // find eyes on face
        if (eyesClassifierLoaded) {
            cv::Mat faceROI = image(face);
            faceROI(cv::Range(0, face.height/2), cv::Range(0, face.width));
            eyeClassifier.detectMultiScale(faceROI, eyes);
        }
        // find smile on face
        if (smileClassifierLoaded) {
            cv::Mat faceROI = image(face);
            faceROI(cv::Range(face.height/2, face.height), cv::Range(0, face.width));
            smileClassifier.detectMultiScale(faceROI, smiles, 1.3, 20);
        }
    }
    for (auto&& face : facesInFrame)
        if (face.size().width > 10 || face.size().height > 10) {
            rectangle(image, face, cv::Scalar(147, 167, 255), 2);
            drawLabel(image, currentClassName, face.x, face.y);
            for (auto&& eye : eyes) {
                cv::Point eye_center(face.x + eye.x + eye.width / 2, face.y + eye.y + eye.height / 2);
                short radius = cvRound((eye.width + eye.height) * 0.25);
                circle(image, eye_center, radius, cv::Scalar(239, 190, 98), 2);
            }
            for (auto&& smile : smiles) {
                rectangle(image, cv::Rect(face.x + smile.x, face.y + smile.y, smile.width, smile.height), cv::Scalar(100, 167, 255), 2);
            }
        }
}

cv::Rect FaceDetector::getLastRect() {
    return facesInFrame.back();
}
cv::Rect ObjectDetector::getLastRect() {
    return lastRect;
}

ObjectDetector::ObjectDetector(detectorProperties props) {
    modelPath = props.modelPath;
    classNamesPath = props.classNamesPath;
    infGraphPath = props.infGraphPath;
    framework = props.framework;
    shouldSwapRB = props.shouldSwapRB;
    meanValues = props.meanValues;
    type = network;
} 
bool ObjectDetector::init() {
    try {
        std::ifstream ifs(classNamesPath);
        std::string line;
        while (getline(ifs, line)) {
            classNames.push_back(line);
        }
    }
    catch (const std::exception& ex) {
        return false;
    }
    try {
        model = cv::dnn::readNet(infGraphPath, modelPath, framework);
        return true;
    }
    catch (const std::exception& ex) {
        return false;
    }
}

void ObjectDetector::detect(cv::Mat& image, bool showConf) {
    cv::Mat blob = cv::dnn::blobFromImage(image, 1.0, cv::Size(320,320), meanValues, shouldSwapRB, false);

    model.setInput(blob);
    blob = model.forward();

    cv::Mat detectionMat(blob.size[2], blob.size[3], CV_32F, blob.ptr<float>());

    for (int i = 0; i < detectionMat.rows; i++) {
        int classId = detectionMat.at<float>(i, 1);
        float confidence = detectionMat.at<float>(i, 2);

        if (confidence > minConfidence) {
            int box_x = (int) (detectionMat.at<float>(i, 3) * image.cols);
            int box_y = (int) (detectionMat.at<float>(i, 4) * image.rows);
            int box_width = (int) (detectionMat.at<float>(i, 5) * image.cols - box_x);
            int box_height = (int) (detectionMat.at<float>(i, 6) * image.rows - box_y);
            lastRect = cv::Rect(box_x, box_y, box_width, box_height);

            cv::rectangle(image, lastRect, cv::Scalar(147, 167, 255), 2);

            currentClassName = classNames[classId - 1].c_str();
            std::stringstream ss;
            ss << currentClassName;
            if (showConf)
                ss << ": confidence = " + std::to_string((int)(confidence * 100)) + "%";
            drawLabel(image, ss.str(), box_x, box_y);
        }
    }
}

void ObjectDetector::setMinConfidence(float c) {
    if (c > 0 && c < 1)
        minConfidence = c;
}
