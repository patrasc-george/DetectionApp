#include <QVBoxLayout>"+"
#include <QPushButton>
#include <QCheckbox>
#include <QComboBox>
#include "menuOptions.h"

Menu::Menu(QWidget* parent)
    : QWidget(parent) {

    setMinimumWidth(150);

    auto* vbox = new QVBoxLayout();

    exit = new QPushButton("Exit");
    toggleCamera = new QPushButton("Turn On");
    toggleEyes = new QCheckBox("Detect eyes");
    detectorsList = new QComboBox;
    showRes = new QCheckBox("Show resolution");
    showFps = new QCheckBox("Show Framerate");

    toggleEyes->setEnabled(false);
    showRes->setEnabled(false);
    showFps->setEnabled(false);

    detectorsList->addItem("No detection");
    detectorsList->addItem("Faces");
    detectorsList->addItem("Objects");
    detectorsList->setCurrentIndex(0);

    vbox->addWidget(toggleCamera);
    vbox->addWidget(detectorsList);
    vbox->addWidget(toggleEyes);
    vbox->addWidget(showRes);
    vbox->addWidget(showFps);

    vbox->addStretch(1);
    vbox->addWidget(exit);

    vbox->setContentsMargins(10, 0, 10, 0);
    setLayout(vbox);
}