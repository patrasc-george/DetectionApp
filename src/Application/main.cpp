#include <iostream>
#include <opencv2/opencv.hpp>

#include <QApplication>

#include "ObjectDetection.h"
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    std::vector<Detector*> detList;

    detectorProperties props;
    props.modelPath = "../data/lbpcascades/lbpcascade_frontalface_improved.xml";
    FaceDetector det = FaceDetector(props, "../data/haarcascades/haarcascade_eye.xml");
    detList.emplace_back(&det);


    detectorProperties props2;
    props2.modelPath = "../data/models/mobilenet_v2/ssd_mobilenet_v2_coco_2018_03_29.pbtxt.txt";
    props2.classNamesPath = "../data/models/mobilenet_v2/object_detection_classes_coco.txt";
    props2.infGraphPath = "../data/models/mobilenet_v2/frozen_inference_graph.pb";
    props2.framework = "TensorFlow";
    props2.meanValues = cv::Scalar(127.5, 127.5, 127.5);
    ObjectDetector det2 = ObjectDetector(props2);
    detList.emplace_back(&det2);

    QApplication app(argc, argv);

    MainWindow window(detList);

    window.setWindowTitle("Detection App");
    window.show();
    window.startVideoCapture();
    return app.exec();
}
