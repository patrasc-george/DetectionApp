#pragma once

#include <opencv2/opencv.hpp>
#include "ObjectDetection.h"

#include <QWidget>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QGraphicsView>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>

#include "menuOptions.h"


class MainWindow : public QWidget {
    Q_OBJECT

public:
    // three main sections
    Menu* menu; // controls
    QGraphicsView* imageContainer;
    QListWidget* statusBar;

public:
    MainWindow(std::vector<Detector*>& detList, QWidget* parent = nullptr);
    ~MainWindow() {};
    void startVideoCapture();

private slots:
    void toggleCameraEvent();
    void selectDetectorEvent();
    void screenshotEvent();
    void changeMinConfEvent();

private:
    bool cameraIsOn = false;
    std::vector<Detector*> detList;
    short detIndex;
};