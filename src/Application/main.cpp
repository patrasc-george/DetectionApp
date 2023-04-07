#include <iostream>
#include <opencv2/opencv.hpp>

#include <QApplication>
#include "ObjectDetection.h"
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    std::vector<Detector*> detList;

    detectorProperties props;
    props.modelPath = std::string(getenv("OpenCV_DIR")) + "/etc/haarcascades/haarcascade_frontalface_alt.xml";
    FaceDetector det = FaceDetector(props);
    detList.emplace_back(&det);

    detectorProperties props2;
    props2.modelPath = "data/yolov5s.onnx";
    ObjectDetector det2 = ObjectDetector(props2);
    detList.emplace_back(&det2);

    QApplication app(argc, argv);
    MainWindow* w = new MainWindow(detList);
    w->show();
    return app.exec();
}
