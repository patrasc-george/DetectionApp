#include "components.h"
#include <QGridLayout>

/**
 * @brief Constructs a LabeledSlider object.
 * @param[in] name The name of the slider.
 * @param[in] min The minimum value of the slider.
 * @param[in] max The maximum value of the slider.
 * @param[in] step The step size of the slider.
 * @param[in] isPercent A boolean indicating whether or not to display the value as a percentage.
 */
LabeledSlider::LabeledSlider(QString name, short min, short max, short step, bool isPercent) {
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
	connect(slider, &QSlider::valueChanged, this, &LabeledSlider::changeLabelValue);
	slider->setValue((int)(min + max) / 2);
}

/**
 * @brief Sets the initial value of the slider.
 * @param[in] value The initial value to set.
 */
void LabeledSlider::setInitialValue(int value) {
	slider->setValue(value);
}

/**
 * @brief Returns the current value of the slider.
 * @return An integer representing the current value of the slider.
 */
int LabeledSlider::value() {
	return slider->value();
}

/**
 * @brief Updates the label text to reflect the current value of the slider.
 */
void LabeledSlider::changeLabelValue() {
	QString text = QString::number(slider->value());
	if (isPercent) text += "%";
	label->setText(text);
};

/**
 * @brief Constructs a SceneImageViewer object.
 */
SceneImageViewer::SceneImageViewer() {
	zoomCount = 0;
	setScene(&m_scene);
	m_scene.addItem(&m_item);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setResizeAnchor(QGraphicsView::AnchorViewCenter);
	setAlignment(Qt::AlignCenter);
	setStyleSheet("border: 0px");
}

/**
 * @brief Sets the pixmap of the image item in the scene.
 * @param[in] pixmap The pixmap to set.
 */
void SceneImageViewer::setPixmap(const QPixmap& pixmap) {
	m_item.setPixmap(pixmap);
	auto offset = -QRectF(pixmap.rect()).center();
	m_item.setOffset(offset);
	setSceneRect(offset.x() * 4, offset.y() * 4, -offset.x() * 8, -offset.y() * 8);
}

/**
 * @brief Zooms in on the image by a specified number of times.
 * @param[in] times The number of times to zoom in.
 */
void SceneImageViewer::zoomIn(int times) {
	QGraphicsView::scale(pow(1.1, times), pow(1.1, times));
	zoomCount += times;
	setDragMode(QGraphicsView::ScrollHandDrag);
}

/**
 * @brief Zooms out on the image by a specified number of times.
 * @param[in] times The number of times to zoom out.
 */
void SceneImageViewer::zoomOut(int times) {
	if (zoomCount > 0) {
		QGraphicsView::scale(pow(1.0 / 1.1, times), pow(1.0 / 1.1, times));
		zoomCount -= times;
		if (zoomCount == 0) {
			setDragMode(QGraphicsView::NoDrag);
		}
	}
}

/**
 * @brief Resets the zoom level of the image to its original state.
 */
void SceneImageViewer::zoomReset() {
	if (zoomCount == 0) return;
	zoomCount = 0;
	setDragMode(QGraphicsView::NoDrag);
	fitInView(&m_item, Qt::KeepAspectRatio);
}

/**
 * @brief Returns the current zoom level of the image.
 * @return An integer representing the current zoom level of the image.
 */
int SceneImageViewer::getZoomCount() {
	return zoomCount;
}
