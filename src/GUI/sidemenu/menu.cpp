#include "menu.h"

#include <fstream>

Menu::Menu(QWidget* parent)
	: QWidget(parent) {

	setFixedWidth(250);
	// arrange controls in a vertical box (as in display:flexbox from CSS)
	auto* vbox = new QVBoxLayout();
	auto* scrollArea = new QScrollArea();

	// initialize controls
	toggleCamera = new QPushButton("Turn On");
	detectorsList = new QComboBox;
	screenshot = new QPushButton("Save screenshot");
	showConfidence = new QCheckBox("Show confidences");
	confControl = new LabeledSlider("Min confidence", 1, 99, 5, true);
	thresholdControl = new LabeledSlider("Threshold", 1, 250, 10);
	cannyThresholdControl = new LabeledSlider("Threshold", 1, 250, 5);
	kernelSizeControl = new LabeledSlider("Kernel", 1, 10, 2);
	uploadButton = new QPushButton("Upload image");

	classButtons = new CollapsibleWidget("Classes");
	imageAlgorithms = new CollapsibleWidget("Image processing");
	binaryThresholdingButton = new QPushButton("Binary Thresholding", imageAlgorithms);
	binaryThresholdingButton->setCheckable(true);
	zeroThresholdingButton = new QPushButton("Thresholding to zero", imageAlgorithms);
	zeroThresholdingButton->setCheckable(true);
	truncThresholdingButton = new QPushButton("Truncate", imageAlgorithms);
	truncThresholdingButton->setCheckable(true);
	adaptiveThresholdingButton = new QPushButton("Adaptive Thresholding", imageAlgorithms);
	adaptiveThresholdingButton->setCheckable(true);
	grayscaleHistogramEqualizationButton = new QPushButton("Grayscale Histogram Equalization", imageAlgorithms);
	grayscaleHistogramEqualizationButton->setCheckable(true);
	colorHistogramEqualizationButton = new QPushButton("Color Histogram Equalization", imageAlgorithms);
	colorHistogramEqualizationButton->setCheckable(true);
	sobelButton = new QPushButton("Sobel", imageAlgorithms);
	sobelButton->setCheckable(true);
	triangleThresholdingButton = new QPushButton("Triangle Thresholding", imageAlgorithms);
	triangleThresholdingButton->setCheckable(true);
	binomialButton = new QPushButton("Binomial", imageAlgorithms);
	binomialButton->setCheckable(true);
	cannyButton = new QPushButton("Canny", imageAlgorithms);
	cannyButton->setCheckable(true);
	openingButton = new QPushButton("Opening", imageAlgorithms);
	openingButton->setCheckable(true);

	editDetectorsBtn = new QPushButton("Edit detectors");

	QVBoxLayout* algVbox = new QVBoxLayout;
	algVbox->addWidget(binaryThresholdingButton);
	algVbox->addWidget(zeroThresholdingButton);
	algVbox->addWidget(truncThresholdingButton);
	algVbox->addWidget(adaptiveThresholdingButton);
	algVbox->addWidget(grayscaleHistogramEqualizationButton);
	algVbox->addWidget(colorHistogramEqualizationButton);
	algVbox->addWidget(sobelButton);
	algVbox->addWidget(triangleThresholdingButton);
	algVbox->addWidget(binomialButton);
	algVbox->addWidget(cannyButton);
	algVbox->addWidget(openingButton);
	imageAlgorithms->setContentLayout(*algVbox);

	detectedClassesVbox = new QVBoxLayout;
	undetectedClassesVbox = new QVBoxLayout;

	QVBoxLayout* classesVbox = new QVBoxLayout;
	classesVbox->addLayout(detectedClassesVbox);
	classesVbox->addLayout(undetectedClassesVbox);

	detectedLabel = new QLabel("Detected");
	undetectedLabel = new QLabel("Undetected");

	detectedClassesVbox->addWidget(detectedLabel);
	undetectedClassesVbox->addWidget(undetectedLabel);

	classButtons->setContentLayout(*classesVbox);

	magnifier = new QPushButton();
	magnifier->setIcon(QIcon(":/assets/magnifier.png"));
	magnifier->setToolTip("Magnifier");

	zoomIn = new QPushButton();
	zoomIn->setIcon(QIcon(":/assets/zoom-in_dark.png"));
	zoomIn->setToolTip("Zoom in");

	zoomOut = new QPushButton();
	zoomOut->setIcon(QIcon(":/assets/zoom-out_dark.png"));
	zoomOut->setToolTip("Zoom out");

	zoomReset = new QPushButton();
	zoomReset->setIcon(QIcon(":/assets/maximize_dark.png"));
	zoomReset->setToolTip("Zoom to fit");

	flipHorizontal = new QPushButton();
	flipHorizontal->setCheckable(true);
	flipHorizontal->setIcon(QIcon(":/assets/horizontal-flip_dark.png"));
	flipHorizontal->setToolTip("Flip horizontally");

	flipVertical = new QPushButton();
	flipVertical->setCheckable(true);
	flipVertical->setIcon(QIcon(":/assets/vertical-flip_dark.png"));
	flipVertical->setToolTip("Flip vertically");

	undoBtn = new QPushButton();
	undoBtn->setIcon(QIcon(":/assets/undo_dark.png"));
	undoBtn->setToolTip("Undo");

	redoBtn = new QPushButton();
	redoBtn->setIcon(QIcon(":/assets/redo_dark.png"));
	redoBtn->setToolTip("Redo");

	// we make the Camera Toggle pushButton behave like a checkbox so we can access its 'checked' state
	toggleCamera->setCheckable(true);
	toggleCamera->setObjectName("CameraToggle");

	detectorsList->addItem("None");

	// we add the controls in our vertical box
	QGridLayout* miniButtons = new QGridLayout;
	miniButtons->addWidget(magnifier, 0, 0);
	miniButtons->addWidget(zoomIn, 0, 1);
	miniButtons->addWidget(zoomOut, 0, 2);
	miniButtons->addWidget(zoomReset, 0, 3);
	miniButtons->addWidget(flipHorizontal, 0, 4);
	miniButtons->addWidget(flipVertical, 0, 5);
	miniButtons->addWidget(undoBtn, 0, 6);
	miniButtons->addWidget(redoBtn, 0, 7);

	vbox->addWidget(toggleCamera);
	vbox->addLayout(miniButtons);

	vbox->addWidget(new QLabel("Select a detector", detectorsList));
	vbox->addWidget(detectorsList);


	QWidget* container = new QWidget;
	scrollArea->setWidget(container);

	QVBoxLayout* lay = new QVBoxLayout(container);
	lay->setAlignment(Qt::AlignTop);
	lay->addWidget(imageAlgorithms);
	lay->addWidget(classButtons);

	scrollArea->setWidgetResizable(true);
	scrollArea->setContentsMargins(0, 0, 0, 0);
	scrollArea->setStyleSheet("QScrollArea { border: none; padding: 0; }");

	vbox->addWidget(scrollArea, Qt::AlignTop);

	vbox->addWidget(showConfidence);
	vbox->addWidget(confControl);
	vbox->addWidget(thresholdControl);
	vbox->addWidget(cannyThresholdControl);
	vbox->addWidget(kernelSizeControl);

	vbox->addStretch(1); // add spacing so the next controls will appear at the bottom of the menu
	vbox->addWidget(uploadButton);
	vbox->addWidget(screenshot);
	vbox->addWidget(editDetectorsBtn);

	setLayout(vbox); // our menu will show the vertical box
}