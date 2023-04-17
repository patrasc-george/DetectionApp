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

	connect(menu->exit, &QPushButton::clicked, qApp, &QApplication::quit);
	connect(menu->toggleCamera, &QPushButton::clicked, this, &MainWindow::toggleCameraEvent);
	connect(menu->detectorsList, &QComboBox::currentIndexChanged, this, &MainWindow::selectDetectorEvent);
	connect(menu->screenshot, &QPushButton::clicked, this, &MainWindow::takeScreenshot);

}

void MainWindow::startVideoCapture() {
	int fps = 0;
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
		if (!cap.read(frame))
			break;

		if (menu->flip->isChecked())
			cv::flip(frame, frame, 1);

		try {
			if (detIndex > 0 && menu->detectorsList->currentIndex() > 0)
				detList[detIndex - 1]->detect(frame, menu->toggleEyes->isChecked());
		}
		catch (const std::exception& ex) {
			QString err = tr("There was an error while loading the detection model: \n%1").arg(*ex.what());
			QMessageBox::critical(this, "Error", err);
			menu->detectorsList->setCurrentIndex(0);
		}
		displayInfo(frame, menu->showRes->isChecked(), menu->showFps->isChecked(), fps);

		// convert the image from OpenCV Mat format to QImage for display in QimageContainer
		QImage* qimg = new QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
		// create a QGraphicsPixmapItem to display the image in the scene
		QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(*qimg));
		// add the item to the scene and adjust the scene size to fit the item size
		scene->addItem(pixmapItem);
		scene->setSceneRect(pixmapItem->boundingRect()); // adjust the scene size to the image size
		// wait for a short interval to reduce resource consumption
		QCoreApplication::processEvents();


		delete qimg;
		delete pixmapItem;
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
	menu->showRes->setEnabled(cameraIsOn);
	menu->showFps->setEnabled(cameraIsOn);
	menu->flip->setEnabled(cameraIsOn);
	menu->screenshot->setEnabled(cameraIsOn);

	startVideoCapture();
}

void MainWindow::selectDetectorEvent() {
	if (menu->detectorsList->currentIndex() != 0)
		detIndex = menu->detectorsList->currentIndex();

	if (menu->detectorsList->currentIndex() != 1)
		menu->toggleEyes->setEnabled(false);
	else
		menu->toggleEyes->setEnabled(cameraIsOn);
}

void MainWindow::takeScreenshot() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"),
		QString(),
		tr("Images (*.png)"));
	if (!fileName.isEmpty())
	{
		QPixmap pixMap = imageContainer->grab(imageContainer->sceneRect().toRect());
		pixMap.save(fileName);
	}
}