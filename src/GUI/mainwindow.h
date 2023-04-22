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
	void setOptions();
	void toggleCameraEvent();
	QString getImageFileName();
	void toggleImageEvent();
	void verifyImageIsUpload();
	void selectDetectorEvent();
	void changeMinConfEvent();
	void screenshotEvent();
	void setDetector();
	void showRes();
	void showFPS(int& fps, int& avgFps, std::deque<int>& fpsArray);
	void flipImage();
	void displayImage();
	void processImage();

private:
	cv::Mat frame;
	QString fileName;
	std::vector<Detector*> detList;
	short detIndex;
	short displayedInfoCount;
	bool cameraIsOn = false;
	bool imageIsUpload = false;
};