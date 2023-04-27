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
	QGraphicsView* imageContainer;
	QListWidget* statusBar;

public:
	MainWindow(std::vector<Detector*>& detList, QWidget* parent = nullptr);
	~MainWindow() {};
	void startVideoCapture();

private slots:
	void toggleCameraEvent();
	void uploadImageEvent();
	void selectDetectorEvent();
	void changeMinConfEvent();
	void screenshotEvent();
	void processImage();

private:
	QGraphicsPixmapItem pixmap;
	cv::Mat frame;
	QString fileName;
	std::vector<Detector*> detList;
	short detIndex;
	short displayedInfoCount;
	bool cameraIsOn = false;
	bool imageIsUpload = false;
	void setOptions();
	void setDetector();
	void showRes();
	void showFPS(int& fps, int& avgFps);
	void flipImage();
	void displayImage();
	QString getImageFileName();
	void binaryThresholdingProcess();
	void histogramEqualizationProcess();
};