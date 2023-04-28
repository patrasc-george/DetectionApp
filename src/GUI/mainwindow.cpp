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
#include <QStatusBar>

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
	statusBar = new QStatusBar();
	resLabel = new QLabel();
	statusBar->addPermanentWidget(resLabel);

	// link the controls defined in our menu to the events of our window
	// syntax: connect(widget that emits a signal, the type of the signal, the object that acts on the signal, the method (slot) that will be called)
	connect(menu->exit, &QPushButton::clicked, this, &MainWindow::close);
	connect(menu->toggleCamera, &QAbstractButton::toggled, this, &MainWindow::toggleCameraEvent);
	connect(menu->uploadButton, &QPushButton::clicked, this, &MainWindow::uploadImageEvent);
	connect(menu->detectorsList, &QComboBox::currentIndexChanged, this, &MainWindow::selectDetectorEvent);
	connect(menu->confControl, &LabeledSlider::valueChanged, this, &MainWindow::changeMinConfEvent);
	connect(menu->toggleEyes, &QCheckBox::toggled, this, &MainWindow::processImage);
	connect(menu->showRes, &QCheckBox::toggled, this, &MainWindow::processImage);
	connect(menu->flip, &QCheckBox::toggled, this, &MainWindow::processImage);
	connect(menu->showConfidence, &QCheckBox::toggled, this, &MainWindow::processImage);
	connect(menu->screenshot, &QPushButton::clicked, this, &MainWindow::screenshotEvent);
	connect(menu->thresholdControl, &LabeledSlider::valueChanged, this, &MainWindow::changeThresholdEvent);

	imageContainer->setFixedSize(640, 480);

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

	// init the PixMap
	imageContainer->setScene(new QGraphicsScene(this));
	imageContainer->scene()->addItem(&pixmap);
	imageContainer->setSceneRect(imageContainer->scene()->sceneRect());
	imageContainer->setAlignment(Qt::AlignCenter);
}

void MainWindow::setOptions()
{
	menu->toggleCamera->setText("Turn " + QString(cameraIsOn ? "Off" : "On"));
	menu->toggleEyes->setVisible((cameraIsOn || imageIsUpload) && detIndex == 1);
	menu->showConfidence->setVisible((cameraIsOn || imageIsUpload) && detIndex == 2);
	menu->confControl->setVisible((cameraIsOn || imageIsUpload) && detIndex == 2);
	menu->showRes->setVisible(cameraIsOn || imageIsUpload);
	menu->showFps->setVisible(cameraIsOn);
	menu->flip->setVisible(cameraIsOn || imageIsUpload);
	menu->screenshot->setVisible(cameraIsOn || imageIsUpload);
	menu->thresholdControl->setVisible((cameraIsOn || imageIsUpload) && detIndex == 3);
}

void MainWindow::toggleCameraEvent() {
	cameraIsOn = menu->toggleCamera->isChecked();
	menu->uploadButton->setChecked(false);
	imageIsUpload = menu->uploadButton->isChecked();
	setOptions();

	if (cameraIsOn) {
		menu->flip->setChecked(true);
		imageContainer->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		imageContainer->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		startVideoCapture();
	}
	else {
		frame.setTo(cv::Scalar(255, 255, 255));
		displayImage();
	}
}

QString MainWindow::getImageFileName()
{
	return QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
}

void MainWindow::uploadImageEvent()
{
	fileName = getImageFileName();

	if (fileName.isEmpty()) {
		return;
	}
	frame = cv::imread(fileName.toStdString());
	statusBar->showMessage(QString("Uploaded file: %1").arg(fileName));

	menu->toggleCamera->setChecked(false);
	menu->flip->setChecked(false);
	imageIsUpload = true;
	setOptions();
	processImage();
	displayImage();
}

void MainWindow::selectDetectorEvent() {
	detIndex = menu->detectorsList->currentIndex();

	setOptions();
	if (imageIsUpload) processImage();
}

void MainWindow::changeMinConfEvent() {
	detList[1]->setMinConfidence(menu->confControl->value() / static_cast<float>(100));
	if (imageIsUpload)
		processImage();
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
		statusBar->showMessage(QString("Saved file: %1").arg(fileName));
	}
}

void MainWindow::setDetector()
{
	// detect if a detector is selected
	try {
		if (detIndex == 1) {
			detList[detIndex - 1]->detect(frame, menu->toggleEyes->isChecked());
		}
		else if (detIndex == 2) {
			detList[detIndex - 1]->detect(frame, menu->showConfidence->isChecked());
		}

		if (detIndex > 0 && detIndex <= 2 && detList[detIndex - 1]->getLastRect().empty() == false) {
			cv::Rect rect = detList[detIndex - 1]->getLastRect();
			statusBar->showMessage(QString("Detected %5 at: <%1 %2> - <%3 %4>")
				.arg(QString::number(rect.x))
				.arg(QString::number(rect.y))
				.arg(QString::number(rect.x + rect.width))
				.arg(QString::number(rect.y + rect.height))
				.arg(QString::fromStdString(detList[detIndex - 1]->currentClassName)));
		}

		else if (detIndex == 3) {
			binaryThresholding(frame, menu->thresholdControl->value());
		}
		else if (detIndex == 4) {
			histogramEqualization(frame);
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

void MainWindow::showFPS(int& fps, int& avgFps)
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

void MainWindow::displayImage() {
	QImage qimg(frame.data, frame.cols, frame.rows, static_cast<int>(frame.step), QImage::Format_BGR888);
	pixmap.setPixmap(QPixmap::fromImage(qimg));
	imageContainer->fitInView(&pixmap, Qt::KeepAspectRatio);
	QString res = QString("Resolution: %1 x %2")
		.arg(QString::number(frame.size().width))
		.arg(QString::number(frame.size().height));
	resLabel->setText(res);
	
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

		processImage();
		showFPS(fps, avgFps);
		displayImage();
	}
	cap.release();
	QCoreApplication::processEvents();
}

void MainWindow::processImage() {
	displayedInfoCount = 0;
	if (imageIsUpload)
		frame = cv::imread(fileName.toStdString());

	flipImage();
	setDetector();
	showRes();

	if (frame.empty())
		return;

	if (imageIsUpload) displayImage();
}

void MainWindow::changeThresholdEvent() {
	if (imageIsUpload)
		processImage();
	statusBar->showMessage(QString("Applied binary thresholding: %1").arg(menu->thresholdControl->value()));
}
