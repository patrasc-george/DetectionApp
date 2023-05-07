#pragma once

#include <QWidget>
#include <QComboBox>

#include "components.h"

class Menu : public QWidget {
public:
	QPushButton* toggleCamera;
	QCheckBox* toggleFaceFeatures;
	QCheckBox* showRes;
	QCheckBox* showFps;
	QCheckBox* showConfidence;
	QComboBox* detectorsList;
	QPushButton* screenshot;
	LabeledSlider* confControl;
	LabeledSlider* thresholdControl;
	QPushButton* uploadButton;
	QPushButton* zoomIn;
	QPushButton* zoomOut;
	QPushButton* zoomReset;
	QPushButton* flipHorizontal;
	QPushButton* flipVertical;
	QPushButton* undoBtn;
	QPushButton* redoBtn;

public:
	Menu(QWidget* parent = nullptr);
};