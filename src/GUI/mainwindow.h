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
	QString fileName;

public:
	MainWindow(std::vector<Detector*>& detList, QWidget* parent = nullptr);
	~MainWindow() {};
	void startVideoCapture();

private slots:
	void setEnabled();
	void toggleCameraEvent();
	void toggleImageEvent();
	void showResEvent();
	void flipEvent();
	void displayImage(const cv::Mat& frame);
	void setFlip(cv::Mat& frame);
	void setDetector(cv::Mat& frame);
	void selectDetectorEvent();
	void screenshotEvent();
	void changeMinConfEvent();
	QString getImageFileName();
	void processImage();

private:
	bool cameraIsOn = false;
	bool imageIsUpload = false;
	std::vector<Detector*> detList;
	short detIndex;

};