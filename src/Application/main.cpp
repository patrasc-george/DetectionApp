#include <iostream>
#include <opencv2/opencv.hpp>

#include <QApplication>

#include "ObjectDetection.h"
#include "mainwindow.h"
#include "ModelLoader.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MainWindow window;

    window.setWindowTitle("Detection App");
    window.show();
    window.menu->toggleCamera->setChecked(true); // camera is on

    return app.exec();
}
