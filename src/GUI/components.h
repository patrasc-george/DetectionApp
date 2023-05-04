#pragma once

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QWidget>

#include <iostream>

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

class SceneImageViewer : public QGraphicsView {
private:
    QGraphicsScene m_scene;
    QGraphicsPixmapItem m_item;
    int zoomCount;
public:
    SceneImageViewer();
    void setPixmap(const QPixmap& pixmap);
    void zoomIn(int times = 1);
    void zoomOut(int times = 1);
    void zoomReset();
	int getZoomCount();
};
