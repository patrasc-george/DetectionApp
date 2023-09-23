#pragma once

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QGridLayout>
#include <QGraphicsScene>

class LabeledSlider : public QWidget {
	Q_OBJECT

private:
	QSlider* slider;
	QLabel* label;
	bool isPercent;

public:
	/**
	 * @brief Constructs a LabeledSlider object.
	 * @details This constructor initializes a LabeledSlider object with the provided parameters.
	 It creates and initializes a label and a slider control. The label displays the name and current value of the slider.
	 The slider control allows the user to select a value within the specified range and step size.
	 If isPercent is true, the value is displayed as a percentage.
	 The label and slider are arranged in a layout and added to the LabeledSlider widget.
	 * @param[in] name The name of the slider.
	 * @param[in] min The minimum value of the slider.
	 * @param[in] max The maximum value of the slider.
	 * @param[in] step The step size of the slider.
	 * @param[in] isPercent A boolean indicating whether or not to display the value as a percentage.
	 */
	LabeledSlider(QString name, short min = 0, short max = 100, short step = 10, bool isPercent = false) {
		label = new QLabel();
		slider = new QSlider(Qt::Horizontal);
		this->isPercent = isPercent;

		slider->setRange(min, max);
		slider->setSingleStep(step);
		slider->setTickPosition(QSlider::TicksBelow);
		slider->setTickInterval(step);
		slider->setContentsMargins(10, 0, 10, 0);

		auto* vbox = new QVBoxLayout();
		auto* hbox = new QHBoxLayout();
		vbox->setContentsMargins(0, 10, 0, 0);

		hbox->addWidget(new QLabel(name));
		hbox->addStretch(1);
		hbox->addWidget(label);
		vbox->addLayout(hbox);
		vbox->addWidget(slider);
		this->setLayout(vbox);

		connect(slider, QOverload<int>::of(&QSlider::valueChanged), this, &LabeledSlider::valueChanged);

		// on mouse released
		connect(slider, &QSlider::sliderReleased, this, [&] { emit sliderReleased(value()); });
		connect(slider, &QSlider::valueChanged, this, [&] {
			changeLabelValue();
			// on arrow key released
			if (!slider->isSliderDown())
				emit sliderReleased(value());
			});
		slider->setValue((int)(min + max) / 2);
	}

	/**
	 * @brief Sets the initial value of the slider.
	 * @details This function sets the initial value of the slider control.
	 It should be called after constructing a LabeledSlider object to set its initial value.
	 * @param[in] value The initial value to set.
	 */
	void setInitialValue(int value) {
		slider->setValue(value);
	}

	/**
	 * @brief Returns the current value of the slider.
	 * @details This function returns the current value of the slider control.
	 It can be called at any time to get the current value selected by the user.
	 * @return An integer representing the current value of the slider.
	 */
	int value() {
		return slider->value();
	}

protected:
	/**
	 * @brief Updates the label text to reflect the current value of the slider.
	 * @details This function updates the text of the label to display the current value of the slider control.
	 If isPercent was set to true when constructing the LabeledSlider object, it also appends a percent sign to the value.
	 This function is called automatically when the value of the slider changes.
	 */
	void changeLabelValue() {
		QString text = QString::number(slider->value());
		if (isPercent) text += "%";
		label->setText(text);
	}

signals:
	/**
	 * @brief Signal handler for the valueChanged signal.
	 * @details This function is a signal handler for the valueChanged signal emitted by a `LabeledSlider`.
	 It is invoked when the slider's value changes. The function internally triggers the corresponding
	 Qt signal using QMetaObject::activate().
	 * @param[in] _t1 The new value of the slider.
	 */
	void valueChanged(int x);


	/**
	 * @brief Signal handler for the sliderReleased signal.
	 * @details This function is a signal handler for the sliderReleased signal emitted by a `LabeledSlider`.
	 * It is invoked when the slider is released after being dragged by the user. The function internally triggers
	 * the corresponding Qt signal using QMetaObject::activate().
	 * @param[in] _t1 The value of the slider when it was released.
	 */
	void sliderReleased(int x);
};
