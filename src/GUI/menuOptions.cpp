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
	confSlider = new QSlider(Qt::Horizontal);
	confLabel = new QLabel("");
	uploadButton = new QPushButton("Upload Image");

	/* the 'label' for minimum confidence is made up of two elements :
		- a static label with text
		- the actual label (confLabel) that changs its value based on the slider value (it's done in mainwindow.cpp)

		- they are wrapped in a horizontal box (confBox)
	*/
	auto* confHbox = new QHBoxLayout();
	confHbox->addWidget(new QLabel("Min confidence"));
	confHbox->addStretch(1);
	confHbox->addWidget(confLabel);
	confHbox->setEnabled(false);
	confLabel->setAlignment(Qt::AlignRight);

	// configuring the slider
	confSlider->setRange(10, 95);
	confSlider->setSingleStep(5);
	confSlider->setTickPosition(QSlider::TicksBelow);
	confSlider->setTickInterval(5);

	// we make the Camera Toggle pushButton behave like a checkbox so we can access its 'checked' state
	toggleCamera->setCheckable(true);

	detectorsList->addItem("No detection");
	detectorsList->addItem("Faces");
	detectorsList->addItem("Objects");

	// we add the controls in our vertical box
	vbox->addWidget(toggleCamera);
	vbox->addWidget(detectorsList);
	vbox->addWidget(toggleEyes);
	vbox->addWidget(showRes);
	vbox->addWidget(showFps);
	vbox->addWidget(flip);
	vbox->addWidget(showConfidence);
	vbox->addLayout(confHbox); // we add the horizontal wrapper
	vbox->addWidget(confSlider);

	vbox->addStretch(1); // add spacing so the next controls will appear at the bottom of the menu
	vbox->addWidget(uploadButton);
	vbox->addWidget(screenshot);
	vbox->addWidget(exit);

	vbox->setContentsMargins(10, 0, 10, 0); // add some whitespace
	setLayout(vbox); // our menu will show the vertical box
}