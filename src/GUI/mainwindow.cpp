#include "mainwindow.h"
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
	this->detList = dList;
	this->detIndex = 0;

	menu = new Menu;

	imageContainer = new QGraphicsView;
	imageContainer->setFixedSize(642, 482);

	statusBar = new QListWidget;
	statusBar->setMaximumHeight(50);

	statusBar->addItem("Status Bar (WIP - currently it's just a list item as placeholder)");

	QGridLayout* grid = new QGridLayout;

	grid->addWidget(imageContainer, 0, 0, 1, 1);
	grid->addWidget(menu, 0, 1, 1, 1);
	grid->addWidget(statusBar, 1, 0, 1, 2);

	grid->setContentsMargins(10, 10, 10, 10);
	setLayout(grid);
	setFixedSize(sizeHint());

	connect(menu->exit, &QPushButton::clicked, this, &MainWindow::close);
	connect(menu->toggleCamera, &QPushButton::clicked, this, &MainWindow::toggleCameraEvent);
	connect(menu->detectorsList, &QComboBox::currentIndexChanged, this, &MainWindow::selectDetectorEvent);
	connect(menu->screenshot, &QPushButton::clicked, this, &MainWindow::screenshotEvent);
	connect(menu->confSlider, &QSlider::valueChanged, this, &MainWindow::changeMinConfEvent);

	menu->confSlider->setValue(60);
}

void MainWindow::startVideoCapture() {
	int fps = 0;
	std::deque<int> fpsArray;
	int avgFps{}; 
	cv::VideoCapture cap(0);

	if (!cap.isOpened())
	{
		qDebug() << "Could not open video camera.";
		return;
	}

	// create a scene to display the captured image from the webcam
	QGraphicsScene* scene = new QGraphicsScene();
	imageContainer->setScene(scene);

	while (cameraIsOn && imageContainer->isVisible())
	{
		cv::Mat frame;

		Timer timer(fps);
		fpsArray.emplace_back(fps);
		if (fpsArray.size() > 60)
			fpsArray.pop_front();
		for (auto&& f : fpsArray) {
			avgFps += f;
		}
		avgFps /= fpsArray.size();

		if (!cap.read(frame))
			break;

		if (menu->flip->isChecked())
			cv::flip(frame, frame, 1);

		try {
			if (detIndex == 1)
				detList[detIndex - 1]->detect(frame, menu->toggleEyes->isChecked());
			else if (detIndex > 1)
				detList[detIndex - 1]->detect(frame, menu->showConfidence->isChecked());
		}
		catch (const std::exception& ex) {
			QString err = tr("There was an error while loading the detection model: \n%1").arg(*ex.what());
			QMessageBox::critical(this, "Error", err);
			menu->detectorsList->setCurrentIndex(0);
		}
		displayInfo(frame, menu->showRes->isChecked(), menu->showFps->isChecked(), fps, avgFps);

		// convert the image from OpenCV Mat format to QImage for display in QimageContainer
		QImage qimg = QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
		QGraphicsPixmapItem pixmapItem = QGraphicsPixmapItem(QPixmap::fromImage(qimg));

		scene->addItem(&pixmapItem);
		//scene->setSceneRect(pixmapItem->boundingRect()); // adjust the scene size to the image size
		QCoreApplication::processEvents();

	}
	cap.release();
	delete scene;
};

void MainWindow::toggleCameraEvent() {
	cameraIsOn = !cameraIsOn;
	if (cameraIsOn)
		menu->toggleCamera->setText("Turn Off");
	else
		menu->toggleCamera->setText("Turn On");
	menu->toggleEyes->setEnabled(cameraIsOn && detIndex == 1);
	menu->showConfidence->setEnabled(cameraIsOn && detIndex > 1);
	menu->confSlider->setEnabled(cameraIsOn && detIndex > 1);
	menu->showRes->setEnabled(cameraIsOn);
	menu->showFps->setEnabled(cameraIsOn);
	menu->flip->setEnabled(cameraIsOn);
	menu->screenshot->setEnabled(cameraIsOn);

	startVideoCapture();
}

void MainWindow::selectDetectorEvent() {
	detIndex = menu->detectorsList->currentIndex();

	menu->toggleEyes->setEnabled(cameraIsOn && detIndex == 1);
	menu->showConfidence->setEnabled(cameraIsOn && detIndex > 1);
	menu->confSlider->setEnabled(cameraIsOn && detIndex > 1);
}

void MainWindow::changeMinConfEvent() {
	detList[1]->setMinConfidence(menu->confSlider->value() / static_cast<float>(100));
	menu->confLabel->setText(QString::number(menu->confSlider->value()) + QString("%"));
}

void MainWindow::screenshotEvent() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"),
		QString(),
		tr("Images (*.png)"));
	if (!fileName.isEmpty())
	{
		QPixmap pixMap = imageContainer->grab(imageContainer->sceneRect().toRect());
		pixMap.save(fileName);
	}
}