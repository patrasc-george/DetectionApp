#pragma once

#include <QWidget>
#include <QComboBox>
#include <QGroupBox>

#include "components.h"

class Menu : public QWidget {
public:
	QPushButton* toggleCamera;
	QCheckBox* toggleFaceFeatures;
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

	QGroupBox* imageAlgorithms;
	QPushButton* binaryThresholdingButton;
	QPushButton* zeroThresholdingButton;
	QPushButton* adaptiveThresholdingButton;
	QPushButton* histogramEqualizationButton;
	QPushButton* detectEdgesButton;


public:
	Menu(QWidget* parent = nullptr);
};