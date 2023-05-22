#include "ObjectDetection.h"

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

void drawLabel(cv::Mat& image, std::string label, int left, int top) {
    int baseLine;
    cv::Size label_size = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.7, 1.5, &baseLine);

    top = std::max(top, label_size.height);
    cv::Point tlc = cv::Point(left + 4, top + label_size.height + 6);
    putText(image, label, tlc, cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(143, 237, 76), 2);
}

FaceDetector::FaceDetector(detectorProperties& props, std::string eyeClassifierPath, std::string smileClassifierPath) {
    currentClassName = "face";
    modelPath = props.modelPath;
    shouldSwapRB = props.shouldSwapRB;
    this->eyeClassifierPath = eyeClassifierPath;
    this->smileClassifierPath = smileClassifierPath;
    type = cascade;
}

int FaceDetector::init() {
    if (modelPath == "\0")
        return -2;
    if (!faceClassifier.load(modelPath))
        return -3;
    if (!eyeClassifierPath.empty())
        eyesClassifierLoaded = eyeClassifier.load(eyeClassifierPath);
    if (!smileClassifierPath.empty())
        smileClassifierLoaded = smileClassifier.load(smileClassifierPath);

    return 1;
}

void FaceDetector::detect(cv::Mat& image, bool showFeatures) {
    if (image.type() == CV_8UC4)
        cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);
    cv::Mat gray;
    if (image.type() != CV_8UC1)
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    else
        gray = image;

    faceClassifier.detectMultiScale(gray, facesInFrame, 1.1, 6);
    if (facesInFrame.size() == 0)
        return;
    for (auto&& face : facesInFrame) {
        if (showFeatures == false)
            break;
        // find eyes on face
        if (eyesClassifierLoaded) {
            cv::Mat faceROI = gray(face);
            faceROI = faceROI(cv::Range(0, face.height / 2), cv::Range(0, face.width));
            eyeClassifier.detectMultiScale(faceROI, eyes);
        }
        // find smile on face
        if (smileClassifierLoaded) {
            cv::Mat faceROI = gray(face);
            faceROI = faceROI(cv::Range(face.height / 2, face.height), cv::Range(0, face.width));
            smileClassifier.detectMultiScale(faceROI, smiles, 1.2, 6);
        }
    }
    for (auto&& face : facesInFrame) {
        if (face.size().width > 10 || face.size().height > 10) {
            rectangle(image, face, cv::Scalar(143, 237, 76), 2);
            drawLabel(image, currentClassName, face.x, face.y);
        }
        if (showFeatures == false)
            break;
        for (auto&& eye : eyes) {
            cv::Point eye_center(face.x + eye.x + eye.width / 2, face.y + eye.y + eye.height / 2);
            short radius = cvRound((eye.width + eye.height) * 0.25);
            circle(image, eye_center, radius, cv::Scalar(239, 190, 98), 2);
        }
        for (auto&& smile : smiles) {
            rectangle(image, cv::Rect(face.x + smile.x, face.y + face.width - smile.y * 2, smile.width, smile.height), cv::Scalar(100, 167, 255), 2);
        }
    }
}

cv::Rect FaceDetector::getLastRect() {
    return facesInFrame.empty() ? cv::Rect() : facesInFrame.back();
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

int ObjectDetector::init() {
    if (modelPath == "\0")
        return -2;
    if (infGraphPath == "\0")
        return -4;
    std::ifstream ifs(classNamesPath);
    std::string line;
    while (getline(ifs, line))
        classNames.push_back(line);
    try {
        model = cv::dnn::readNet(infGraphPath, modelPath, framework);
    }
    catch (cv::Exception&) {
        return -5;
    }
    return 1;
}

void ObjectDetector::detect(cv::Mat& image, bool showConf) {
    if (image.type() == CV_8UC4)
        cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);
    cv::Mat blob = cv::dnn::blobFromImage(image, 1.0, cv::Size(320, 320), meanValues, shouldSwapRB, false);
    std::vector<std::string> layers = model.getLayerNames();

    model.setInput(blob);
    try {
        /*
        In Debug mode, the forward method requires a string argument representing the layer,
        otherwise it throws a fatal exception and the whole app crashes.
        In Release mode, it automatically chooses the right layer
        */
#ifdef NDEBUG
        blob = model.forward();
#else
        blob = model.forward("layer");
#endif
    }
    catch (const std::exception& e) {
        std::cout << e.what()<<std::endl;
        std::exception ex("No valid layer was provided to model.forward(). This would happen if the application is run in Debug mode.");
        throw ex;
        return;
    }

    cv::Mat detectionMat(blob.size[2], blob.size[3], CV_32F, blob.ptr<float>());

    for (int i = 0; i < detectionMat.rows; i++) {
        int classId = detectionMat.at<float>(i, 1);
        float confidence = detectionMat.at<float>(i, 2);

        if (confidence > minConfidence) {
            int box_x = (int)(detectionMat.at<float>(i, 3) * image.cols);
            int box_y = (int)(detectionMat.at<float>(i, 4) * image.rows);
            int box_width = (int)(detectionMat.at<float>(i, 5) * image.cols - box_x);
            int box_height = (int)(detectionMat.at<float>(i, 6) * image.rows - box_y);
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
