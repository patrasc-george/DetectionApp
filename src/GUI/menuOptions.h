#pragma once

#include <QWidget>
#include <QComboBox>

#include "components.h"

class Menu : public QWidget {
public:
	QPushButton* exit;
	QPushButton* toggleCamera;
	QCheckBox* toggleFaceFeatures;
	QCheckBox* showRes;
	QCheckBox* showFps;
	QCheckBox* flip;
	QCheckBox* showConfidence;
	QComboBox* detectorsList;
	QPushButton* screenshot;
	LabeledSlider* confControl;
	LabeledSlider* thresholdControl;
	QPushButton* uploadButton;

public:
	Menu(QWidget* parent = nullptr);
};