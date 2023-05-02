#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckbox>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include "menuOptions.h"

Menu::Menu(QWidget* parent)
	: QWidget(parent) {

	setMinimumWidth(150);

	// arrange controls in a vertical box (as in display:flexbox from CSS)
	auto* vbox = new QVBoxLayout();

	// initialize controls
	exit = new QPushButton("Exit");
	toggleCamera = new QPushButton("Turn On");
	toggleEyes = new QCheckBox("Detect eyes");
	detectorsList = new QComboBox;
	showRes = new QCheckBox("Show resolution");
	showFps = new QCheckBox("Show framerate");
	screenshot = new QPushButton("Save screenshot");
	flip = new QCheckBox("Flip image");
	showConfidence = new QCheckBox("Show confidences");
	confControl = new LabeledSlider("Min confidence", 5, 95, 5, true);
	thresholdControl = new LabeledSlider("Threshold", 1, 250, 10);
	uploadButton = new QPushButton("Upload Image");


	// we make the Camera Toggle pushButton behave like a checkbox so we can access its 'checked' state
	toggleCamera->setCheckable(true);

	detectorsList->addItem("No detection");
	//detectorsList->addItem("Binary Thresholding");
	//detectorsList->addItem("Histogram Equalization");


	// we add the controls in our vertical box
	vbox->addWidget(toggleCamera);
	vbox->addWidget(detectorsList);
	vbox->addWidget(flip);
	vbox->addWidget(showRes);
	vbox->addWidget(showFps);
	vbox->addWidget(toggleEyes);
	vbox->addWidget(showConfidence);
	vbox->addWidget(confControl); // we add the horizontal wrapper
	vbox->addWidget(thresholdControl);

	vbox->addStretch(1); // add spacing so the next controls will appear at the bottom of the menu
	vbox->addWidget(uploadButton);
	vbox->addWidget(screenshot);
	vbox->addWidget(exit);

	vbox->setContentsMargins(10, 0, 10, 0); // add some whitespace
	setLayout(vbox); // our menu will show the vertical box
}