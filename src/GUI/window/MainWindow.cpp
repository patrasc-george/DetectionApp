﻿#include "MainWindow.h"

#include "DetectorFactory.h"
#include "DetectionMat.h"
#include "ThresholdAdjuster.h"
#include "CanToggleObjects.h"
#include "NeuralNetworkDetector.h"
#include "CascadeClassifierDetector.h"
#include "CascadeClassifierGroup.h"

#include <iostream>

void MainWindow::closeEvent(QCloseEvent* event) {
	if (currDet != nullptr && !currDet->getSerializationFile().empty())
		currDet->serialize(currDet->getSerializationFile());
	// close the entire application
	qApp->closeAllWindows();
	qApp->exit(0);
	exit(0);
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	// instantiate the list of detectors
	this->currDet = nullptr;

	// instantiate the menu (see constructor), image container and status bar
	menu = new Menu;
	imageContainer = new SceneImageViewer;
	statusBar = new QStatusBar();
	resLabel = new QLabel();
	fpsLabel = new QLabel();
	statusBar->addPermanentWidget(resLabel);
	statusBar->addPermanentWidget(fpsLabel);
	statusBar->setSizeGripEnabled(false);
	statusBar->setFixedHeight(35);
	statusBar->setContentsMargins(0, 0, 20, 0);

	// link the controls defined in our menu to the events of our window
	// syntax: connect(widget that emits a signal, the type of the signal, the object that acts on the signal, the method (slot) that will be called)
	connect(menu->confControl, &LabeledSlider::valueChanged, this, &MainWindow::changeMinConfEvent);
	connect(menu->toggleCamera, &QAbstractButton::toggled, this, &MainWindow::toggleCameraEvent);
	connect(menu->uploadButton, &QPushButton::clicked, this, &MainWindow::uploadImageEvent);
	connect(menu->detectorsList, &QComboBox::currentIndexChanged, this, &MainWindow::selectDetectorEvent);
	connect(menu->screenshot, &QPushButton::clicked, this, &MainWindow::screenshotEvent);

	//connect(menu->confControl, &LabeledSlider::sliderReleased, this, &MainWindow::changeMinConfEvent);
	connect(menu->thresholdControl, &LabeledSlider::valueChanged, this, &MainWindow::changeThresholdEvent);
	connect(menu->showConfidence, &QCheckBox::clicked, this, [&] {
		history.add(SHOW_CONFIDENCE, menu->showConfidence->isChecked());
		statusBar->showMessage(QString("Toggled show confidences %1").arg(menu->showConfidence->isChecked() ? "on" : "off"));
		processImage();
		});

	connect(menu->histogramEqualizationButton, &QPushButton::clicked, this, [&] {
		history.add(HISTOGRAM_EQUALIZATION, menu->histogramEqualizationButton->isChecked());
		processImage();
		});

	connect(menu->binaryThresholdingButton, &QPushButton::clicked, this, [&] {
		if (menu->binaryThresholdingButton->isChecked())
			history.add(BINARY_THRESHOLDING, menu->thresholdControl->value());
		else
			history.add(BINARY_THRESHOLDING, 0);
		processImage();
		});

	connect(menu->truncThresholdingButton, &QPushButton::clicked, this, [&] {
		if (menu->truncThresholdingButton->isChecked())
			history.add(TRUNC_THRESHOLDING, menu->thresholdControl->value());
		else
			history.add(TRUNC_THRESHOLDING, 0);
		processImage();
		});

	connect(menu->adaptiveThresholdingButton, &QPushButton::clicked, this, [&] {
		if (menu->adaptiveThresholdingButton->isChecked())
			history.add(ADAPTIVE_THRESHOLDING, menu->thresholdControl->value());
		else
			history.add(ADAPTIVE_THRESHOLDING, 0);
		processImage();
		});

	connect(menu->zeroThresholdingButton, &QPushButton::clicked, this, [&] {
		if (menu->zeroThresholdingButton->isChecked())
			history.add(ZERO_THRESHOLDING, menu->thresholdControl->value());
		else
			history.add(ZERO_THRESHOLDING, 0);
		processImage();
		});

	connect(menu->detectEdgesButton, &QPushButton::clicked, this, [&] {
		history.add(DETECT_EDGES, menu->detectEdgesButton->isChecked());
		processImage();
		});

	connect(menu->flipHorizontal, &QCheckBox::clicked, this, [&] {
		history.add(FLIP_HORIZONTAL, menu->flipHorizontal->isChecked());
		statusBar->showMessage("Flipped horizontally");
		processImage();
		});
	connect(menu->flipVertical, &QCheckBox::clicked, this, [&] {
		history.add(FLIP_VERTICAL, menu->flipVertical->isChecked());
		statusBar->showMessage("Flipped vertically");
		processImage();
		});
	connect(menu->undoBtn, &QPushButton::clicked, this, [&] {
		history.undo();
		statusBar->showMessage(QString("Undone %1").arg(history.lastChange().c_str()));
		processImage();
		setOptions();
		});
	connect(menu->redoBtn, &QPushButton::clicked, this, [&] {
		history.redo();
		statusBar->showMessage(QString("Redone %1").arg(history.lastChange().c_str()));
		processImage();
		setOptions();
		});

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
	connect(menu->editDetectorsBtn, &QPushButton::clicked, [&] {
		// open the detectors editor
		//DetectorsList* editor = new DetectorsList(modelsJSON);
		//editor->show();
		//connect(editor, &DetectorsList::detectorRemoved, this, &MainWindow::detectorEditEvent);
		//connect(editor, &DetectorsList::detectorAdded, this, &MainWindow::detectorEditEvent);
		//connect(editor, &DetectorsList::detectorEdited, this, &MainWindow::detectorEditEvent);
		});

	connect(menu->classButtons->toggleButton, &QToolButton::toggled, this, &MainWindow::sortButtons);

	imageContainer->setMinimumSize(800, 600);

	QVBoxLayout* vbox = new QVBoxLayout;
	QHBoxLayout* hbox = new QHBoxLayout;
	hbox->addWidget(imageContainer);
	hbox->addWidget(menu, 0);
	vbox->addLayout(hbox);
	vbox->addWidget(statusBar);
	hbox->setContentsMargins(20, 0, 20, 0);
	layout()->setContentsMargins(0, 20, 0, 0);

	// set the initial values of the menu controls
	menu->flipHorizontal->setChecked(true); // the image is flipped
	menu->detectorsList->setCurrentIndex(0); // 0 = no detection, 1... = other options

	// init the PixMap
	imageContainer->setScene(new QGraphicsScene);
	imageContainer->scene()->addItem(&pixmap);
	imageContainer->setAlignment(Qt::AlignCenter);

	QWidget* centralWidget = new QWidget;
	centralWidget->setLayout(vbox);
	setCentralWidget(centralWidget);

	QDir folder("../detector_paths");
	QStringList files = folder.entryList({ "*.yaml" }, QDir::Files);

	for (const QString& filename : files) {
		QString filePath = folder.absoluteFilePath(filename);

		Detector* detector = DetectorFactory::createDetectorFromFile(filePath.toStdString());
		if (detector)
			menu->detectorsList->addItem(QFileInfo(filePath).baseName());
	}
}

void MainWindow::setOptions()
{
	menu->detectorsList->setEnabled(cameraIsOn || imageIsUpload);
	menu->toggleCamera->setText("   Turn Camera " + QString(cameraIsOn ? "Off" : "On"));
	menu->showConfidence->setVisible((cameraIsOn || imageIsUpload) && currDet != nullptr && currDet->toThresholdAdjuster());
	menu->confControl->setVisible((cameraIsOn || imageIsUpload) && currDet != nullptr && currDet->toThresholdAdjuster());
	menu->flipHorizontal->setEnabled(cameraIsOn || imageIsUpload);
	menu->flipVertical->setEnabled(cameraIsOn || imageIsUpload);
	menu->screenshot->setVisible(cameraIsOn || imageIsUpload);
	menu->thresholdControl->setVisible((cameraIsOn || imageIsUpload) && thresholdActive());
	menu->zoomIn->setEnabled(imageIsUpload);
	menu->zoomOut->setEnabled(imageIsUpload && (imageContainer->getZoomCount() > 0));
	menu->zoomReset->setEnabled(menu->zoomOut->isEnabled());

	menu->undoBtn->setEnabled(history.canUndo());
	menu->redoBtn->setEnabled(history.canRedo());

	menu->showConfidence->setChecked(history.get()->getShowConfidence());
	menu->flipHorizontal->setChecked(history.get()->getFlipH());
	menu->flipVertical->setChecked(history.get()->getFlipV());
	menu->binaryThresholdingButton->setChecked(history.get()->getBinaryThresholdingValue());
	menu->zeroThresholdingButton->setChecked(history.get()->getZeroThresholdingValue());
	menu->truncThresholdingButton->setChecked(history.get()->getTruncThresholdingValue());
	menu->adaptiveThresholdingButton->setChecked(history.get()->getAdaptiveThresholdingValue());
	menu->histogramEqualizationButton->setChecked(history.get()->getHistogramEqualization());
	menu->detectEdgesButton->setChecked(history.get()->getDetectEdges());

	menu->binaryThresholdingButton->setEnabled(
		!menu->zeroThresholdingButton->isChecked() &&
		!menu->adaptiveThresholdingButton->isChecked()
	);
	menu->zeroThresholdingButton->setEnabled(
		!menu->binaryThresholdingButton->isChecked() &&
		!menu->adaptiveThresholdingButton->isChecked()
	);
	menu->adaptiveThresholdingButton->setEnabled(
		!menu->binaryThresholdingButton->isChecked() &&
		!menu->zeroThresholdingButton->isChecked()
	);

	menu->imageAlgorithms->setVisible(cameraIsOn || imageIsUpload);

	menu->classButtons->setVisible((cameraIsOn || imageIsUpload) && currDet != nullptr && currDet->toObjectToggler());

	menu->detectedLabel->setVisible(!dynamic_cast<CascadeClassifierGroup*>(currDet));
	menu->undetectedLabel->setVisible(!dynamic_cast<CascadeClassifierGroup*>(currDet));
}

void MainWindow::toggleCameraEvent() {
	cameraIsOn = menu->toggleCamera->isChecked();
	menu->uploadButton->setChecked(false);
	imageIsUpload = menu->uploadButton->isChecked();
	setOptions();
	menu->toggleCamera->clearFocus();
	history.reset();
	imageContainer->zoomReset();

	if (cameraIsOn) {
		// only update min confidence when slider is released
		menu->confControl->signalMode = LabeledSlider::OnRelease;

		menu->flipHorizontal->setChecked(true);
		history.get()->setFlipH(menu->flipHorizontal->isChecked());
		menu->flipVertical->setChecked(false);
		history.get()->setFlipV(menu->flipVertical->isChecked());

		startVideoCapture();
		selectDetectorEvent();

	}
	else {
		frame = putLogo(imageContainer->size().width(), imageContainer->size().height());
		displayImage();
		delete currDet;
		currDet = nullptr;
	}
}

QString MainWindow::getImageFileName() {
	return QFileDialog::getOpenFileName(this, tr("Open Image"), QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first(), tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
}

void MainWindow::uploadImageEvent() {
	QString temp = getImageFileName();

	if (temp.isEmpty())
		return;

	QImage backup_frame = frame;
	frame = QImage(temp);
	if (frame.isNull()) {
		QMessageBox::critical(this, "Error", QString("Couldnt't read image from %1. The file may be corrupted or not a valid image file.").arg(fileName));
		frame = backup_frame;
		return;
	}
	fileName = temp;

	statusBar->showMessage(QString("Uploaded file: %1").arg(fileName));

	menu->toggleCamera->setChecked(false);
	menu->flipHorizontal->setChecked(false);
	menu->flipVertical->setChecked(false);

	history.reset();
	history.get()->setFlipH(menu->flipHorizontal->isChecked());
	history.get()->setFlipV(menu->flipVertical->isChecked());

	// update min confidence whenever slider is updated, since it doesn't affect performance
	menu->confControl->signalMode = LabeledSlider::OnChange;

	imageIsUpload = true;
	imageContainer->zoomReset();
	setOptions();
	processImage();
	imageContainer->fitInView(&pixmap, Qt::KeepAspectRatio);
	displayImage();
}

void MainWindow::selectDetectorEvent() {
	menu->classButtons->toggle(false);
	if (currDet != nullptr && !currDet->getSerializationFile().empty()) {
		currDet->serialize(currDet->getSerializationFile());
	}
	delete currDet;
	currDet = nullptr;
	if (menu->detectorsList->currentIndex() == 0) {
		setOptions();
		if (imageIsUpload)
			processImage();
		return;
	}

	QString currText = menu->detectorsList->currentText() + QString(".yaml");

	currDet = DetectorFactory::createDetectorFromFile("../detector_paths/" + currText.toStdString());

	// TODO: add back error messages

	if (currDet->toThresholdAdjuster())
		changeMinConfEvent();
	if (auto det = currDet->toObjectToggler()) {
		// Identify and remove non-label widgets (separators) from the layout
		for (int i = menu->detectedClassesVbox->count() - 1; i >= 0; --i) {
			if (!dynamic_cast<QLabel*>(menu->detectedClassesVbox->itemAt(i)->widget())) {
				QLayoutItem* item = menu->detectedClassesVbox->takeAt(i);
				if (QWidget* widget = item->widget()) {
					delete widget;
				}
				delete item;
			}
		}
		for (int i = menu->undetectedClassesVbox->count() - 1; i >= 0; --i) {
			if (!dynamic_cast<QLabel*>(menu->undetectedClassesVbox->itemAt(i)->widget())) {
				QLayoutItem* item = menu->undetectedClassesVbox->takeAt(i);
				if (QWidget* widget = item->widget()) {
					delete widget;
				}
				delete item;
			}
		}

		auto classes = det->getObjectLabels();
		bool primaryFound = false;
		for (auto c : classes) {
			QPushButton* b = new QPushButton(QString::fromStdString(c));
			b->setCheckable(true);
			b->setChecked(det->isObjectEnabled(c));
			menu->undetectedClassesVbox->addWidget(b);
			menu->buttonMap[c] = b;

			if (primaryFound)
				continue;

			if (auto det = dynamic_cast<CascadeClassifierGroup*>(currDet)) {
				if (c == det->getPrimary()) {
					b->setChecked(true);
					b->setEnabled(false);
					b->setIcon(QIcon::fromTheme("noEntry"));
					primaryFound = false;
				}
			}
		}
		for (QPushButton* button : menu->classButtons->findChildren<QPushButton*>()) {
			connect(button, &QPushButton::clicked, this, &MainWindow::processImage);
		}
	}
	
	if (imageIsUpload)
		processImage();
	setOptions();
}

void MainWindow::changeMinConfEvent() {
	if (auto* det = currDet->toThresholdAdjuster())
		det->adjustThreshold(menu->confControl->value() / static_cast<float>(100));
	if (imageIsUpload)
		processImage();
	sortButtons();
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

void MainWindow::setDetector() {
	if (currDet == nullptr)
		return;
	try {
		cv::Mat mat;
		ConvertQImage2Mat(frame, mat);
		if (dynamic_cast<NeuralNetworkDetector*>(currDet) && mat.type() == CV_8UC1) {
			QMessageBox::critical(this, "Error", "This detector does not work on 1-channel images");
			menu->detectorsList->setCurrentIndex(0);
			return;
		}

		if (auto det = currDet->toObjectToggler()) {
			for (QPushButton* btn : menu->classButtons->findChildren<QPushButton*>()) {
				det->enableObject(btn->text().toStdString(), btn->isChecked());
			}
		}

		detMat = currDet->detect(mat);
		detMat.setShowConfidence(menu->showConfidence->isChecked());
		for (auto& det : detMat) {
			det.setColor(generateColorFromString(det.getLabel()));
		}
		detMat.render(mat);
		ConvertMat2QImage(mat, frame);

		std::vector<Detection> dets = detMat.getAll();
		if (!dets.empty()) {
			cv::Rect rect = dets.back().getRect();
			statusBar->showMessage(QString("Detected %5 at: <%1 %2> - <%3 %4>")
				.arg(QString::number(rect.x))
				.arg(QString::number(rect.y))
				.arg(QString::number(rect.x + rect.width))
				.arg(QString::number(rect.y + rect.height))
				.arg(QString::fromStdString(dets.back().getLabel())));
		}
		else statusBar->clearMessage();

	}
	catch (const std::exception& e) {
		QMessageBox::critical(this, "Error", e.what());
		menu->detectorsList->setCurrentIndex(0);
	}
}

void MainWindow::flipImage() {
	frame = frame.mirrored(menu->flipHorizontal->isChecked(), menu->flipVertical->isChecked());
}

void MainWindow::displayImage() {
	pixmap.setPixmap(QPixmap::fromImage(frame));
	imageContainer->scene()->setSceneRect(imageContainer->scene()->itemsBoundingRect());

	preventReset();

	QString res = QString("Resolution: %1 x %2  ")
		.arg(QString::number(frame.size().width()))
		.arg(QString::number(frame.size().height()));
	if (!cameraIsOn && !imageIsUpload) {
		resLabel->setText("");
		fpsLabel->setText("");
	}
	else
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
	cv::Mat mat;
	while (cameraIsOn && imageContainer->isVisible()) {
		// measure live fps, create a queue of 60 measurements and find the average value
		Timer timer(fps);
		fpsArray.emplace_back(fps);
		if (fpsArray.size() > 60)
			fpsArray.pop_front();
		for (auto&& f : fpsArray)
			avgFps += f;
		avgFps /= fpsArray.size();

		if (!cap.read(mat))
			break;
		ConvertMat2QImage(mat, frame);
		processImage();
		fpsLabel->setText(QString("FPS: %1   (avg: %2)  ").arg(QString::number(fps)).arg(QString::number(avgFps)));
		displayImage();
	}
	cap.release();
}

void MainWindow::processImage() {
	if (imageIsUpload)
		frame = QImage(fileName);

	selectAlgorithmsEvent();
	flipImage();
	setDetector();

	if (frame.isNull())
		return;

	if (imageIsUpload)
		displayImage();
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

void MainWindow::selectAlgorithmsEvent() {
	setOptions();

	bool algActive = false;
	for (QPushButton* btn : menu->imageAlgorithms->findChildren<QPushButton*>())
		if (btn->isChecked()) {
			algActive = true;
			break;
		}
	if (!algActive)
		return;

	cv::Mat mat;
	ConvertQImage2Mat(frame, mat);
	ProcessingAlgorithms::applyingAlgorithms(mat, history.get(), menu->thresholdControl->value());
	ConvertMat2QImage(mat, frame);
}

void MainWindow::detectorEditEvent() {
	// refresh the detector list

	// delete every detector except the first one (None)
	menu->detectorsList->setCurrentIndex(0);
	for (int i = menu->detectorsList->count() - 1; i > 0; i--)
		menu->detectorsList->removeItem(i);
	//QStringList names = ModelLoader::getNames(modelsJSON);

	//for (auto&& name : names)
	//	menu->detectorsList->addItem(name);

	//// if the current detector is not in the list, set it to None
	//if (names.contains(menu->detectorsList->currentText()) == false)
	//	menu->detectorsList->setCurrentIndex(0);
}

void MainWindow::resizeEvent(QResizeEvent* event) {
	preventReset();
}

bool MainWindow::thresholdActive() {
	return (
		menu->binaryThresholdingButton->isChecked() ||
		menu->zeroThresholdingButton->isChecked() ||
		menu->adaptiveThresholdingButton->isChecked() ||
		menu->truncThresholdingButton->isChecked()
		);
}

void MainWindow::sortButtons() {
	if (!menu->classButtons->expanded())
		return;

	detMat.sortByConfidence();
	std::vector<std::string> classNames = currDet->toObjectToggler()->getObjectLabels();
	std::sort(classNames.begin(), classNames.end());

	// exclude detected classes from the classNames
	for (auto& det : detMat) {
		auto d = std::find(classNames.begin(), classNames.end(), det.getLabel());
		if (d != classNames.end()) {
			classNames.erase(d);
		}
	}

	// add detected classes, sorted by confidence
	for (auto& det : detMat) {
		auto buttonIterator = menu->buttonMap.find(det.getLabel());

		if (buttonIterator != menu->buttonMap.end()) {
			QPushButton* button = buttonIterator->second;
			menu->detectedClassesVbox->removeWidget(button);
			menu->detectedClassesVbox->addWidget(button);
		}
	}
	if (detMat.empty()) {
		menu->detectedLabel->setText("<html><font color='black'>Detected</font><br/><font color='gray'> -- None</font></html>");
	}
	else {
		menu->detectedLabel->setText("Detected");
	}

	// add the rest of the classes, sorted alphabetically
	for (auto& c : classNames) {
		auto buttonIterator = menu->buttonMap.find(c);

		if (buttonIterator != menu->buttonMap.end()) {
			QPushButton* button = buttonIterator->second;
			menu->undetectedClassesVbox->removeWidget(button);
			menu->undetectedClassesVbox->addWidget(button);
		}
	}
	if (classNames.empty()) {
		menu->undetectedLabel->setText("<html><font color='black'>Unetected</font><br/><font color='gray'> -- None</font></html>");
	}
	else {
		menu->undetectedLabel->setText("Undetected");
	}

}
