#include "components.h"
#include <QGridLayout>

/**
 * @brief Constructs a LabeledSlider object.
 * @param[in] name The name of the slider.
 * @param[in] min The minimum value of the slider.
 * @param[in] max The maximum value of the slider.
 * @param[in] step The step size of the slider.
 * @param[in] isPercent A boolean indicating whether or not to display the value as a percentage.
 * @details This constructor initializes a LabeledSlider object with the provided parameters. It creates and initializes a label and a slider control. The label displays the name and current value of the slider. The slider control allows the user to select a value within the specified range and step size. If isPercent is true, the value is displayed as a percentage. The label and slider are arranged in a layout and added to the LabeledSlider widget.
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
 * @details This function sets the initial value of the slider control. It should be called after constructing a LabeledSlider object to set its initial value.
 */
void LabeledSlider::setInitialValue(int value) {
	slider->setValue(value);
}

/**
 * @brief Returns the current value of the slider.
 * @return An integer representing the current value of the slider.
 * @details This function returns the current value of the slider control. It can be called at any time to get the current value selected by the user.
 */
int LabeledSlider::value() {
	return slider->value();
}

/**
 * @brief Updates the label text to reflect the current value of the slider.
 * @details This function updates the text of the label to display the current value of the slider control. If isPercent was set to true when constructing the LabeledSlider object, it also appends a percent sign to the value. This function is called automatically when the value of the slider changes.
 */
void LabeledSlider::changeLabelValue() {
	QString text = QString::number(slider->value());
	if (isPercent) text += "%";
	label->setText(text);
};

/**
 * @brief Constructs a SceneImageViewer object.
 * @details This constructor initializes a SceneImageViewer object. It creates and initializes a QGraphicsScene and QGraphicsPixmapItem. These objects are used to display an image in a scrollable and zoomable view. The SceneImageViewer object also keeps track of its current zoom level and provides functions for zooming in, zooming out, and resetting its zoom level.
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
 * @details This function sets the pixmap of the QGraphicsPixmapItem in the QGraphicsScene. It also adjusts various properties such as offset and scene rect to ensure that the image is displayed correctly in its original size and position within the view.
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
 * @details This function zooms in on an image by scaling it up by a factor of 1.1 for each specified time. It also increments its internal zoom count by that number of times and enables scroll hand drag mode so that users can pan around on large images that do not fit within their viewports after being zoomed in on.
 */
void SceneImageViewer::zoomIn(int times) {
	QGraphicsView::scale(pow(1.1, times), pow(1.1, times));
	zoomCount += times;
	setDragMode(QGraphicsView::ScrollHandDrag);
}

/**
 * @brief Zooms out on an image by a specified number of times.
 * @param[in] times The number of times to zoom out.
 * @details This function zooms out on an image by scaling it down by a factor of 1/1.1 for each specified time. It also decrements its internal zoom count by that number of times and disables scroll hand drag mode if its zoom count reaches zero so that users cannot pan around on images that fit within their viewports after being zoomed out on completely.
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
 * @brief Resets an image's zoom level to its original state (i.e., fits it within its viewport).
 * @details This function resets an image's zoom level by fitting it within its viewport while maintaining its aspect ratio. It also sets its internal zoom count to zero and disables scroll hand drag mode so that users cannot pan around on images that fit within their viewports after being reset completely.
 */
void SceneImageViewer::zoomReset() {
	if (zoomCount == 0) return;
	zoomCount = 0;
	setDragMode(QGraphicsView::NoDrag);
	fitInView(&m_item, Qt::KeepAspectRatio);
}

/**
 * @brief Returns an image's current zoom level (i.e., how many times it has been zoomed in or out).
 * @return An integer representing an image's current zoom level (positive for zoomed in, negative for zoomed out).
 * @details This function returns an image's current zoom level as tracked by its internal zoom count variable. A positive value indicates that it has been zoomed in on while a negative value indicates that it has been zoomed out on (zero indicates no zoom).
 */
int SceneImageViewer::getZoomCount() {
	return zoomCount;
}
