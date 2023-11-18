#pragma once

#include "../custom_widgets/LabeledSlider.hpp"
#include "../custom_widgets/CollapsibleWidget.hpp"

#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckbox>

class Menu : public QWidget {
public:
	QPushButton* toggleCamera;
	QCheckBox* showConfidence;
	QComboBox* detectorsList;
	QPushButton* screenshot;
	LabeledSlider* confControl;
	LabeledSlider* thresholdControl;
	LabeledSlider* kernelSizeControl;
	LabeledSlider* cannyThresholdControl;
	QPushButton* uploadButton;

	QPushButton* magnifier;
	QPushButton* zoomIn;
	QPushButton* zoomOut;
	QPushButton* zoomReset;

	QPushButton* flipHorizontal;
	QPushButton* flipVertical;

	QPushButton* undoBtn;
	QPushButton* redoBtn;

	QVBoxLayout* detectedClassesVbox;
	QVBoxLayout* undetectedClassesVbox;
	QLabel* detectedLabel;
	QLabel* undetectedLabel;
	CollapsibleWidget* classButtons;
	std::map<std::string, QPushButton*> buttonMap;

	CollapsibleWidget* imageAlgorithms;
	QPushButton* binaryThresholdingButton;
	QPushButton* zeroThresholdingButton;
	QPushButton* truncThresholdingButton;
	QPushButton* adaptiveThresholdingButton;
	QPushButton* grayscaleHistogramEqualizationButton;
	QPushButton* colorHistogramEqualizationButton;
	QPushButton* sobelButton;
	QPushButton* triangleThresholdingButton;
	QPushButton* binomialButton;
	QPushButton* cannyButton;
	QPushButton* editDetectorsBtn;

public:
	/**
	 * @brief Constructs a Menu object.
	 * @details This constructor initializes a Menu object with the provided parent widget.
	 It creates and initializes various controls such as buttons, checkboxes, combo boxes, sliders, and group boxes.
	 These controls are used to interact with the application and control its behavior.
	 The controls are arranged in a vertical box layout and added to the Menu widget.
	 * @param[in] parent The parent widget of the Menu object.
	 */
	Menu(QWidget* parent = nullptr);
};