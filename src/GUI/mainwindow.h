#pragma once

#include <opencv2/opencv.hpp>
#include "ObjectDetection.h"

//#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
//#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
//#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

#include "menuOptions.h"


class MainWindow : public QWidget {
    Q_OBJECT

public:
    Menu* menu;
    QGraphicsView* imageContainer;
    QListWidget* statusBar;

public:
    MainWindow(std::vector<Detector*>& detList, QWidget* parent = nullptr);
    ~MainWindow() {};
    void startVideoCapture();

private slots:
    void toggleCameraEvent();
    void selectDetectorEvent();
    void takeScreenshot();

private:
    bool cameraIsOn = true;
    std::vector<Detector*> detList;
    short detIndex;
};