#include <iostream>
#include <opencv2/opencv.hpp>

#include <QApplication>

#include "ObjectDetection.h"
#include "mainwindow.h"
#include "ModelLoader.h"

int main(int argc, char* argv[]) {
    std::vector<Detector*> detList;

    ModelLoader::load("../data/models.json", detList);

    QApplication app(argc, argv);

    MainWindow window(detList);

    window.setWindowTitle("Detection App");
    window.show();
    window.menu->toggleCamera->setChecked(true); // camera is on

    return app.exec();
}
