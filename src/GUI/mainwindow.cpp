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
#include <QStandardPaths>

#include "ObjectDetection.h"
#include "CameraInteraction.h"
#include "../Application/ModelLoader.h"

#define modelsJSON "../data/detectors.json"
QVector<QString> names = ModelLoader::getNames(modelsJSON);


MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {

	// instantiate the list of detectors
	this->currDet = nullptr;
	displayedInfoCount = 0;

	// instantiate the menu (see constructor), image container and status bar
	menu = new Menu;
	imageContainer = new SceneImageViewer;
	statusBar = new QStatusBar();
	resLabel = new QLabel();
	statusBar->addPermanentWidget(resLabel);
	statusBar->setSizeGripEnabled(false);
	statusBar->setFixedHeight(35);
	statusBar->setContentsMargins(0, 0, 20, 0);

	// link the controls defined in our menu to the events of our window
	// syntax: connect(widget that emits a signal, the type of the signal, the object that acts on the signal, the method (slot) that will be called)
	connect(menu->toggleCamera, &QAbstractButton::toggled, this, &MainWindow::toggleCameraEvent);
	connect(menu->uploadButton, &QPushButton::clicked, this, &MainWindow::uploadImageEvent);
	connect(menu->detectorsList, &QComboBox::currentIndexChanged, this, &MainWindow::selectDetectorEvent);
	connect(menu->confControl, &LabeledSlider::valueChanged, this, &MainWindow::changeMinConfEvent);
	connect(menu->toggleFaceFeatures, &QCheckBox::toggled, this, &MainWindow::processImage);
	connect(menu->showRes, &QCheckBox::toggled, this, &MainWindow::processImage);
	connect(menu->showConfidence, &QCheckBox::toggled, this, &MainWindow::processImage);
	connect(menu->screenshot, &QPushButton::clicked, this, &MainWindow::screenshotEvent);
	connect(menu->thresholdControl, &LabeledSlider::valueChanged, this, &MainWindow::changeThresholdEvent);
	connect(menu->flipHorizontal, &QCheckBox::toggled, this, &MainWindow::processImage);
	connect(menu->flipVertical, &QCheckBox::toggled, this, &MainWindow::processImage);
	connect(menu->zoomIn, &QPushButton::clicked, [&] {
		imageContainer->zoomIn(); 	
		setOptions();
		});
	connect(menu->zoomOut, &QPushButton::clicked, [&] {
		imageContainer->zoomOut();
		if (imageContainer->getZoomCount() == 0)
			imageContainer->fitInView(&pixmap, Qt::KeepAspectRatio);
		setOptions();
		});
	connect(menu->zoomReset, &QPushButton::clicked, [&] {
		imageContainer->zoomReset();
		imageContainer->fitInView(&pixmap, Qt::KeepAspectRatio);
		setOptions();
		});

	imageContainer->setMinimumSize(640, 480);

	QVBoxLayout* vbox = new QVBoxLayout;
	QHBoxLayout* hbox = new QHBoxLayout;
	hbox->addWidget(imageContainer);
	hbox->addWidget(menu, 0);
	vbox->addLayout(hbox);
	vbox->addWidget(statusBar);
	hbox->setContentsMargins(20, 0, 20, 0);
	setLayout(vbox);
	layout()->setContentsMargins(0, 20, 0, 0);

	// set the initial values of the menu controls
	menu->flipHorizontal->setChecked(true); // the image is flipped
	menu->detectorsList->setCurrentIndex(0); // 0 = no detection, 1... = other options

	// init the PixMap
	imageContainer->setScene(new QGraphicsScene);
	imageContainer->scene()->addItem(&pixmap);
	imageContainer->setAlignment(Qt::AlignCenter);
	
	// load labels for detectors (after being seleced they might be deleted if the corresponding detector didn't load succesfully)
	for (QString name : names) {
		menu->detectorsList->addItem(name);
	}
}

void MainWindow::setOptions()
{
	menu->detectorsList->setEnabled(cameraIsOn || imageIsUpload);
	menu->toggleCamera->setText("   Turn Camera " + QString(cameraIsOn ? "Off" : "On"));
	menu->toggleFaceFeatures->setVisible((cameraIsOn || imageIsUpload) && currDet != nullptr && currDet->getType() == cascade && (currDet->canDetectEyes() || currDet->canDetectSmiles()));
	menu->showConfidence->setVisible((cameraIsOn || imageIsUpload) && currDet != nullptr && currDet->getType() == network);
	menu->confControl->setVisible((cameraIsOn || imageIsUpload) && currDet != nullptr && currDet->getType() == network);
	menu->showRes->setVisible(cameraIsOn || imageIsUpload);
	menu->showFps->setVisible(cameraIsOn);
	menu->flipHorizontal ->setEnabled(cameraIsOn || imageIsUpload);
	menu->flipVertical->setEnabled(cameraIsOn || imageIsUpload);
	menu->screenshot->setVisible(cameraIsOn || imageIsUpload);
	//menu->thresholdControl->setVisible((cameraIsOn || imageIsUpload) && detIndex == 3);
	menu->thresholdControl->setVisible(false);
	menu->zoomIn->setEnabled(imageIsUpload);
	menu->zoomOut->setEnabled(imageIsUpload && (imageContainer->getZoomCount() > 0));
	menu->zoomReset->setEnabled(menu->zoomOut->isEnabled());

	menu->undoBtn->setEnabled(false); // temporary
	menu->redoBtn->setEnabled(false); // temporary
}

void MainWindow::toggleCameraEvent() {
	cameraIsOn = menu->toggleCamera->isChecked();
	menu->uploadButton->setChecked(false);
	imageIsUpload = menu->uploadButton->isChecked();
	setOptions();
	menu->toggleCamera->clearFocus();
	imageContainer->zoomReset();

	if (cameraIsOn) {
		menu->flipHorizontal->setChecked(true);
		menu->flipVertical->setChecked(false);
		startVideoCapture();
		selectDetectorEvent();
	}
	else {
		QImage img(imageContainer->size().width(), imageContainer->size().height(), QImage::Format::Format_RGB32);
		QImage logo(":/assets/camera_dark.png");
		logo = logo.scaled(100, 100, Qt::KeepAspectRatio);

		QPainter p;
		img.fill(Qt::white);
		p.begin(&img);
		p.drawImage(imageContainer->size().width() / 2.0 - logo.size().width() / 2.0, imageContainer->size().height() / 2.0 - logo.size().height() / 2.0, logo);
		p.drawText(0, imageContainer->size().height() / 2.0 + logo.size().height() / 2.0 + 20, imageContainer->size().width(), 10, Qt::AlignCenter, "Camera is unavailable");
		p.end();
		cv::Mat  mat(img.height(), img.width(), CV_8UC4, const_cast<uchar*>(img.bits()), static_cast<size_t>(img.bytesPerLine()));

		cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);
		frame = mat;

		displayImage();
		delete currDet;
		currDet = nullptr;
	}
}

QString MainWindow::getImageFileName()
{
	return QFileDialog::getOpenFileName(this, tr("Open Image"), QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first(), tr("Image Files (*.png *.jpg *.bmp)"));
}

void MainWindow::uploadImageEvent() {
	fileName = getImageFileName();

	if (fileName.isEmpty()) {
		return;
	}
	cv::Mat image = cv::imread(fileName.toStdString());
	if (image.data != NULL)
		frame = image;
	else {
		QMessageBox::critical(this, "Error", QString("Couldnt't read image from %1. The file may be corrupted or not a valid image file.").arg(fileName));
		return;
	}
	statusBar->showMessage(QString("Uploaded file: %1").arg(fileName));

	menu->toggleCamera->setChecked(false);
	menu->flipHorizontal->setChecked(false);
	menu->flipVertical->setChecked(false);
	imageIsUpload = true;
	imageContainer->zoomReset();
	setOptions();
	processImage();
	imageContainer->fitInView(&pixmap, Qt::KeepAspectRatio);
	displayImage();
}

void MainWindow::selectDetectorEvent() {
	if (currDet != nullptr) {
		delete currDet;
		currDet = nullptr;
	}
	if (menu->detectorsList->currentIndex() != 0) {
		QString currText = menu->detectorsList->currentText();
		int index = menu->detectorsList->findText(currText);
		if (!ModelLoader::getFromFileByName(currDet, currText, modelsJSON)) {
			QMessageBox::critical(this, "Error", "The selected detector could did not load succesfully! You might need to check if the right files are provided and the paths are set accordingly in detectors.json");
			menu->detectorsList->setItemIcon(index, QIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical)));
			menu->detectorsList->setCurrentIndex(0);
		}
		else {
			menu->detectorsList->setItemIcon(index, QIcon());
		}
	}
	setOptions();
	if (imageIsUpload)
		processImage();
}

void MainWindow::changeMinConfEvent() {
	currDet->setMinConfidence(menu->confControl->value() / static_cast<float>(100));
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
	if (currDet == nullptr) return;
	//isGrayscale = false;
	// detect if a detector is selected
	try {
		if (currDet->getType() == cascade) {
			currDet->detect(frame, menu->toggleFaceFeatures->isChecked());
		}
		else if (currDet->getType() == network) {
			currDet->detect(frame, menu->showConfidence->isChecked());
		}

		if (currDet->getLastRect().empty() == false) {
			cv::Rect rect = currDet->getLastRect();
			statusBar->showMessage(QString("Detected %5 at: <%1 %2> - <%3 %4>")
				.arg(QString::number(rect.x))
				.arg(QString::number(rect.y))
				.arg(QString::number(rect.x + rect.width))
				.arg(QString::number(rect.y + rect.height))
				.arg(QString::fromStdString(currDet->currentClassName)));
		}
		else statusBar->clearMessage();
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
	if (menu->flipHorizontal->isChecked()) {
		cv::flip(frame, frame, 1);
	}
	if (menu->flipVertical->isChecked()) {
		cv::flip(frame, frame, 0);
	}
}

void MainWindow::displayImage() {
	QImage qimg;
	if (isGrayscale)
		qimg = QImage(frame.data, frame.cols, frame.rows, static_cast<int>(frame.step), QImage::Format_Grayscale8);
	else
		qimg = QImage(frame.data, frame.cols, frame.rows, static_cast<int>(frame.step), QImage::Format_BGR888);

	pixmap.setPixmap(QPixmap::fromImage(qimg));
	imageContainer->scene()->setSceneRect(imageContainer->scene()->itemsBoundingRect());
	
	preventReset();
	
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
	isGrayscale = false;

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
	isGrayscale = false;

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

void MainWindow::preventReset() {
	// prevent the container from losing the zoom applying other actions on the image (e.g. flip) or on resize
	imageContainer->fitInView(&pixmap, Qt::KeepAspectRatio);
	if (imageContainer->getZoomCount() > 0) {
		int temp = imageContainer->getZoomCount();
		imageContainer->zoomReset();
		imageContainer->zoomIn(temp);
	}
}