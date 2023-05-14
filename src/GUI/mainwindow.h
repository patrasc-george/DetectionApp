#pragma once

#include <opencv2/opencv.hpp>
#include "ObjectDetection.h"

#include <QWidget>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>

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

public slots:
	void toggleCameraEvent();
	void uploadImageEvent();
	void selectDetectorEvent();
	void changeMinConfEvent();
	void screenshotEvent();
	void processImage();
	void changeThresholdEvent();
	void selectAlgorithmsEvent();

public:
	QGraphicsPixmapItem pixmap;
	cv::Mat frame;
	QString fileName;
	Detector* currDet;
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
	bool thresholdActive() {
		return (
			menu->binaryThresholdingButton->isChecked() ||
			menu->zeroThresholdingButton->isChecked() ||
			menu->adaptiveThresholdingButton->isChecked()
			);
	}
};