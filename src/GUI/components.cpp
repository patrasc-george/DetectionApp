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

void SceneImageViewer::setPixmap(const QPixmap& pixmap) {
	m_item.setPixmap(pixmap);
	auto offset = -QRectF(pixmap.rect()).center();
	m_item.setOffset(offset);
	setSceneRect(offset.x() * 4, offset.y() * 4, -offset.x() * 8, -offset.y() * 8);
}

void SceneImageViewer::zoomIn(int times) {
	QGraphicsView::scale(pow(1.1, times), pow(1.1, times));
	zoomCount += times;
	setDragMode(QGraphicsView::ScrollHandDrag);
}

void SceneImageViewer::zoomOut(int times) {
	if (zoomCount > 0) {
		QGraphicsView::scale(pow(1.0 / 1.1, times), pow(1.0 / 1.1, times));
		zoomCount -= times;
		if (zoomCount == 0) {
			setDragMode(QGraphicsView::NoDrag);
		}
	}
}

void SceneImageViewer::zoomReset() {
	if (zoomCount == 0) return;
	zoomCount = 0;
	setDragMode(QGraphicsView::NoDrag);
	fitInView(&m_item, Qt::KeepAspectRatio);
}

int SceneImageViewer::getZoomCount() {
	return zoomCount;
}
