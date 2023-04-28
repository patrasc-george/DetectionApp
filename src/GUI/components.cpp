#include "components.h"
#include <QGridLayout>

LabeledSlider::LabeledSlider(QString name, short min, short max, short step, bool isPercent) {
	label = new QLabel();
	slider = new QSlider(Qt::Horizontal);
	this->isPercent = isPercent;

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

	connect(slider, QOverload<int>::of(&QSlider::valueChanged), this, &LabeledSlider::valueChanged);
	connect(slider, &QSlider::valueChanged, this, &LabeledSlider::changeLabelValue);
	slider->setValue((int)(min + max) / 2);
}
void LabeledSlider::setInitialValue(int value) {
	slider->setValue(value);
}
int LabeledSlider::value() {
	return slider->value();
}
void LabeledSlider::changeLabelValue() {
	QString text = QString::number(slider->value());
	if (isPercent) text += "%";
	label->setText(text);
};