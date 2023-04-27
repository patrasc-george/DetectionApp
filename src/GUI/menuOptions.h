#pragma once

#include <QWidget>
#include <QComboBox>
#include <QLabel>

class Menu : public QWidget {
public:
	QPushButton* exit;
	QPushButton* toggleCamera;
	QCheckBox* toggleEyes;
	QCheckBox* showRes;
	QCheckBox* showFps;
	QCheckBox* flip;
	QCheckBox* showConfidence;
	QComboBox* detectorsList;
	QPushButton* screenshot;
	QSlider* confSlider;
	QLabel* confLabel;
	QPushButton* uploadButton;

public:
	Menu(QWidget* parent = nullptr);
};