#pragma once

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPixMap>

class SceneImageViewer : public QGraphicsView {
private:
	QGraphicsScene m_scene;
	QGraphicsPixmapItem m_item;
	int zoomCount;

public:
	/**
	 * @brief Constructs a SceneImageViewer object.
	 * @details This constructor initializes a SceneImageViewer object.
	 It creates and initializes a QGraphicsScene and QGraphicsPixmapItem.
	 These objects are used to display an image in a scrollable and zoomable view.
	 The SceneImageViewer object also keeps track of its current zoom level
	 and provides functions for zooming in, zooming out, and resetting its zoom level.
	 */
	SceneImageViewer() {
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
	 * @details This function sets the pixmap of the QGraphicsPixmapItem in the QGraphicsScene.
	 It also adjusts various properties such as offset and scene rect to ensure that the image
	 is displayed correctly in its original size and position within the view.
	 */
	void setPixmap(const QPixmap& pixmap) {
		m_item.setPixmap(pixmap);
		auto offset = -QRectF(pixmap.rect()).center();
		m_item.setOffset(offset);
		setSceneRect(offset.x() * 4, offset.y() * 4, -offset.x() * 8, -offset.y() * 8);
	}

	/**
	 * @brief Zooms in on the image by a specified number of times.
	 * @details This function zooms in on an image by scaling it up by a factor of 1.1 for each specified time.
	 It also increments its internal zoom count by that number of times and enables scroll hand drag mode so
	 that users can pan around on large images that do not fit within their viewports after being zoomed in on.
	 */
	void zoomIn(int times = 1) {
		QGraphicsView::scale(pow(1.1, times), pow(1.1, times));
		zoomCount += times;
		setDragMode(QGraphicsView::ScrollHandDrag);
	}

	/**
	 * @brief Zooms out on an image by a specified number of times.
	 * @details This function zooms out on an image by scaling it down by a factor of 1/1.1 for each specified time.
	 It also decrements its internal zoom count by that number of times and disables scroll hand drag mode
	 if its zoom count reaches zero so that users cannot pan around on images that fit within their viewports after being zoomed out on completely.
	 */
	void zoomOut(int times = 1) {
		if (zoomCount > 0) {
			QGraphicsView::scale(pow(1.0 / 1.1, times), pow(1.0 / 1.1, times));
			zoomCount -= times;
			if (zoomCount == 0)
				setDragMode(QGraphicsView::NoDrag);
		}
	}

	/**
	 * @brief Resets an image's zoom level to its original state (i.e., fits it within its viewport).
	 * @details This function resets an image's zoom level by fitting it within its viewport while maintaining its aspect ratio.
	 It also sets its internal zoom count to zero and disables scroll hand drag mode so
	 that users cannot pan around on images that fit within their viewports after being reset completely.
	 */
	void zoomReset() {
		if (zoomCount == 0)
			return;
		zoomCount = 0;
		setDragMode(QGraphicsView::NoDrag);
		fitInView(&m_item, Qt::KeepAspectRatio);
	}

	/**
	 * @brief Returns an image's current zoom level (i.e., how many times it has been zoomed in or out).
	 * @details This function returns an image's current zoom level as tracked by its internal zoom count variable.
	 A positive value indicates that it has been zoomed in on while a negative value indicates that it has been zoomed out on (zero indicates no zoom).
	 */
	int getZoomCount() {
		return zoomCount;
	}
};