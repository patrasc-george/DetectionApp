#pragma once

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>

class labeledSlider : public QWidget {
	Q_OBJECT
private:
	QSlider* slider;
	QLabel* label;
	bool isPercent;
public:
	explicit labeledSlider(QString name, short min = 0, short max = 100, short step = 10, bool isPercent = false) {
		label = new QLabel();
		slider = new QSlider(Qt::Horizontal);
		this->isPercent = isPercent;

		label->setVisible(true);
		slider->setVisible(true);

		slider->setRange(min, max);
		slider->setSingleStep(step);
		slider->setTickPosition(QSlider::TicksBelow);
		slider->setTickInterval(step);

		auto* vbox = new QVBoxLayout();
		auto* hbox = new QHBoxLayout();
		hbox->addWidget(new QLabel(name));
		hbox->addStretch(1);
		hbox->addWidget(label);
		vbox->addLayout(hbox);
		vbox->addWidget(slider);
		this->setLayout(vbox);
		
		connect(slider, QOverload<int>::of(&QSlider::valueChanged), this, &labeledSlider::valueChanged);
		connect(slider, &QSlider::valueChanged, this, &labeledSlider::changeLabelValue);
		slider->setValue((int)(min + max) / 2);
	}
	void setInitialValue(int value) {
		slider->setValue(value);
	}
	int value() {
		return slider->value();
	}
protected:
	void changeLabelValue() {
		QString text = QString::number(slider->value());
		if (isPercent) text += "%";
		label->setText(text);
	}
signals:
	void valueChanged(int x);
};

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
	labeledSlider* confControl;
	labeledSlider* thresholdControl;
	QPushButton* uploadButton;

public:
	Menu(QWidget* parent = nullptr);
};