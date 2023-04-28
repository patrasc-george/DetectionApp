#pragma once

#include <QWidget>
#include <QLabel>
#include <QSlider>

class LabeledSlider : public QWidget {
	Q_OBJECT
private:
	QSlider* slider;
	QLabel* label;
	bool isPercent;
public:
	LabeledSlider(QString name, short min = 0, short max = 100, short step = 10, bool isPercent = false);
	void setInitialValue(int value);
	int value();
protected:
	void changeLabelValue();
signals:
	void valueChanged(int x);
};