#pragma once

#include <opencv2/opencv.hpp>
#include "ObjectDetection.h"
#include "CameraInteraction.h"
//#include "FrameOptions.h"

#include <QWidget>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <stack>

#include "menuOptions.h"


class MainWindow : public QWidget {
	Q_OBJECT

public:
	// three main sections
	Menu* menu; // controls
	SceneImageViewer* imageContainer;
	QStatusBar* statusBar;
	QLabel* resLabel;
	QLabel* fpsLabel;

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow() {};
	void startVideoCapture();

private slots:
	void toggleCameraEvent();
	void uploadImageEvent();
	void selectDetectorEvent();
	void changeMinConfEvent();
	void screenshotEvent();
	void processImage();
	void changeThresholdEvent();
	void selectAlgorithmsEvent();

private:
	QGraphicsPixmapItem pixmap;
	cv::Mat frame;
	QString fileName;
	Detector* currDet;
	OptionsStack statusStack;
	bool cameraIsOn = false;
	bool imageIsUpload = false;
	bool isGrayscale = false;
	void setOptions();
	void setDetector();
	void flipImage();
	void displayImage();
	QString getImageFileName();
	void preventReset();
	void resizeEvent(QResizeEvent* event) override {
		preventReset();
	}
};