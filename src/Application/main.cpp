#include <iostream>
#include <opencv2/opencv.hpp>

#include <QApplication>

#include "ObjectDetection.h"
#include "mainwindow.h"
#include "ModelLoader.h"
#include <QFile>
#include <QTimer>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QTimer::singleShot(0, []() {
        MainWindow window;
        window.setObjectName("window");

        QFile file(":/assets/stylesheet.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(styleSheet);

        window.setWindowTitle("Detection App");
        window.show();
        window.setWindowIcon(QIcon(":/assets/camera_dark.png"));
        window.menu->toggleCamera->setChecked(true); // camera is on

        QCoreApplication::exit(0);
        });
    return app.exec();
}
