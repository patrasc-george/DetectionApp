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

private:
	QGraphicsPixmapItem pixmap;
	cv::Mat frame;
	QString fileName;
	Detector* currDet;
	short displayedInfoCount;
	bool cameraIsOn = false;
	bool imageIsUpload = false;
	bool isGrayscale = false;
	void setOptions();
	void setDetector();
	void showRes();
	void showFPS(int& fps, int& avgFps);
	void flipImage();
	void displayImage();
	QString getImageFileName();
	void resizeEvent(QResizeEvent* event) override {
		if (cameraIsOn || imageIsUpload)
			imageContainer->fitInView(&pixmap, Qt::KeepAspectRatio);
		if (imageContainer->getZoomCount() > 0) {
			int temp = imageContainer->getZoomCount();
			imageContainer->zoomReset();
			imageContainer->zoomIn(temp);
		}
		imageContainer->setAlignment(Qt::AlignCenter);
	}
};