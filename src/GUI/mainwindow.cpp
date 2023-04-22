﻿#include "mainwindow.h"
#include "menuOptions.h"

#include <opencv2/opencv.hpp>

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QThread>
#include <QListWidget>
#include <QMessageBox>
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QFileDialog>

#include "ObjectDetection.h"
#include "CameraInteraction.h"


MainWindow::MainWindow(std::vector<Detector*>& dList, QWidget* parent) : QWidget(parent) {
	// instantiate the list of detectors
	this->detList = dList;
	this->detIndex = 0;
	displayedInfoCount = 0;

	// instantiate the menu (see constructor), image container and status bar
	menu = new Menu;
	imageContainer = new QGraphicsView;
	statusBar = new QListWidget;

	// link the controls defined in our menu to the events of our window
	// syntax: connect(widget that emits a signal, the type of the signal, the object that acts on the signal, the method (slot) that will be called)
	connect(menu->exit, &QPushButton::clicked, this, &MainWindow::close);
	connect(menu->toggleCamera, &QAbstractButton::toggled, this, &MainWindow::toggleCameraEvent);
	connect(menu->uploadButton, &QPushButton::clicked, this, &MainWindow::toggleImageEvent);
	connect(menu->detectorsList, &QComboBox::currentIndexChanged, this, &MainWindow::selectDetectorEvent);
	connect(menu->confSlider, &QSlider::valueChanged, this, &MainWindow::changeMinConfEvent);
	connect(menu->toggleEyes, &QCheckBox::stateChanged, this, &MainWindow::verifyImageIsUpload);
	connect(menu->showRes, &QCheckBox::stateChanged, this, &MainWindow::verifyImageIsUpload);
	connect(menu->flip, &QCheckBox::stateChanged, this, &MainWindow::verifyImageIsUpload);
	connect(menu->showConfidence, &QCheckBox::stateChanged, this, &MainWindow::verifyImageIsUpload);
	connect(menu->screenshot, &QPushButton::clicked, this, &MainWindow::screenshotEvent);

	imageContainer->setFixedSize(642, 482);
	statusBar->setMaximumHeight(50);
	statusBar->addItem("Status Bar (WIP - currently it's just a list item as placeholder)");

	// create a grid that will contain the 3 main components
	QGridLayout* grid = new QGridLayout;
	grid->addWidget(imageContainer, 0, 0, 1, 1);
	grid->addWidget(menu, 0, 1, 1, 1);
	grid->addWidget(statusBar, 1, 0, 1, 2);
	// give the grid some whitespace around
	grid->setContentsMargins(10, 10, 10, 10);
	// the main window will show the grid
	setLayout(grid);
	setFixedSize(sizeHint());

	// set the initial values of the menu controls
	menu->flip->setChecked(true); // the image is flipped
	menu->detectorsList->setCurrentIndex(0); // 0 = no detection, 1 = face detection, 2 = object detection
	menu->confSlider->setValue(60);
}

void MainWindow::setOptions()
{
	menu->toggleCamera->setText("Turn " + QString(cameraIsOn ? "Off" : "On"));
	menu->toggleEyes->setEnabled((cameraIsOn || imageIsUpload) && detIndex == 1);
	menu->showConfidence->setEnabled((cameraIsOn || imageIsUpload) && detIndex > 1);
	menu->confSlider->setEnabled((cameraIsOn || imageIsUpload) && detIndex > 1);
	menu->showRes->setEnabled(cameraIsOn || imageIsUpload);
	menu->showFps->setEnabled(cameraIsOn);
	menu->flip->setEnabled(cameraIsOn || imageIsUpload);
	menu->screenshot->setEnabled(cameraIsOn || imageIsUpload);
}

void MainWindow::toggleCameraEvent() {
	cameraIsOn = menu->toggleCamera->isChecked();
	menu->uploadButton->setChecked(false);
	imageIsUpload = menu->uploadButton->isChecked();
	menu->flip->setChecked(true);
	setOptions();

	if (cameraIsOn) startVideoCapture();
	else delete imageContainer->scene();
}

QString MainWindow::getImageFileName()
{
	return QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
}

void MainWindow::toggleImageEvent()
{
	fileName = getImageFileName();

	if (fileName.isEmpty()) {
		return;
	}

	menu->toggleCamera->setChecked(false);
	cameraIsOn = menu->toggleCamera->isChecked();
	menu->flip->setChecked(false);
	imageIsUpload = true;
	setOptions();
	processImage();
}

void MainWindow::verifyImageIsUpload()
{
	if (imageIsUpload) processImage();
}

void MainWindow::selectDetectorEvent() {
	detIndex = menu->detectorsList->currentIndex();

	menu->toggleEyes->setEnabled((cameraIsOn || imageIsUpload) && detIndex == 1);
	menu->showConfidence->setEnabled((cameraIsOn || imageIsUpload) && detIndex > 1);
	menu->confSlider->setEnabled((cameraIsOn || imageIsUpload) && detIndex > 1);
	verifyImageIsUpload();
}

void MainWindow::changeMinConfEvent() {
	detList[1]->setMinConfidence(menu->confSlider->value() / static_cast<float>(100));
	menu->confLabel->setText(QString::number(menu->confSlider->value()) + QString("%"));
	verifyImageIsUpload();
}

void MainWindow::screenshotEvent() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"),
		QString(),
		tr("Images (*.png)"));
	if (!fileName.isEmpty()) {
		// Re-shrink the scene to it's bounding contents
		imageContainer->scene()->setSceneRect(imageContainer->scene()->itemsBoundingRect());
		// Create the image with the exact size of the shrunk scene
		QImage image(imageContainer->scene()->sceneRect().size().toSize(), QImage::Format_ARGB32);
		// Start all pixels transparent to avoid white background
		image.fill(Qt::transparent); 

		QPainter painter(&image); // paint the image over the transparent pixels
		imageContainer->scene()->render(&painter);
		image.save(fileName);
	}
}

void MainWindow::setDetector()
{
	// detect if a detector is selected
	try {
		if (detIndex == 1) {
			detList[detIndex - 1]->detect(frame, menu->toggleEyes->isChecked());
		}
		else if (detIndex > 1) {
			detList[detIndex - 1]->detect(frame, menu->showConfidence->isChecked());
		}
	}
	catch (const std::exception& ex)
	{
		QString err = tr("There was an error while loading the detection model: \n%1").arg(ex.what());
		QMessageBox::critical(this, "Error", err);
		menu->detectorsList->setCurrentIndex(0);
	}
}

void MainWindow::showRes()
{
	if (menu->showRes->isChecked()) {
		displayInfo(frame, "Resolution", std::to_string(frame.size().width) + "x" + std::to_string(frame.size().height), cv::Point(10, 30 + displayedInfoCount * 30));
		displayedInfoCount++;
	}
}

void MainWindow::showFPS(int& fps, int& avgFps, std::deque<int>& fpsArray)
{
	if (menu->showFps->isChecked()) {
		displayInfo(frame, "FPS", std::to_string(fps), cv::Point(10, 30 + displayedInfoCount * 30));
		displayedInfoCount++;
		displayInfo(frame, "Average FPS", std::to_string(avgFps), cv::Point(10, 30 + displayedInfoCount * 30));
		displayedInfoCount++;
	}
}

void MainWindow::flipImage()
{
	if (menu->flip->isChecked()) {
		cv::flip(frame, frame, 1);
	}
}

void MainWindow::displayImage()
{
	// create a scene to display the captured image from the webcam
	QGraphicsScene* scene = new QGraphicsScene();
	imageContainer->setScene(scene);

	QImage qimg(frame.data, frame.cols, frame.rows, static_cast<int>(frame.step), QImage::Format_BGR888);
	QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(qimg));
	scene->addItem(pixmapItem);
	imageContainer->fitInView(pixmapItem, Qt::KeepAspectRatio);

	QCoreApplication::processEvents();
}

void MainWindow::startVideoCapture() {
	int fps = 0, avgFps = 0;
	std::deque<int> fpsArray;
	cv::VideoCapture cap(0);

	if (!cap.isOpened()) {
		qDebug() << "Could not open video camera.";
		return;
	}

	while (cameraIsOn && imageContainer->isVisible()) {
		displayedInfoCount = 0;

		// measure live fps, create a queue of 60 measurements and find the average value
		Timer timer(fps);
		fpsArray.emplace_back(fps);
		if (fpsArray.size() > 60)
			fpsArray.pop_front();
		for (auto&& f : fpsArray)
			avgFps += f;
		avgFps /= fpsArray.size();

		if (!cap.read(frame))
			break;

		flipImage();
		setDetector();
		showRes();
		showFPS(fps, avgFps, fpsArray);
		displayImage();
	}
	cap.release();
}

void MainWindow::processImage()
{
	displayedInfoCount = 0;
	frame = cv::imread(fileName.toStdString());

	flipImage();
	setDetector();
	showRes();

	if (frame.empty())
		return;

	displayImage();
}