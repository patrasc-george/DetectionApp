#pragma once

#include <QLabel>
#include <QSlider>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGridLayout>
#include <QToolButton>

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
	LabeledSlider(QString name, short min = 0, short max = 100, short step = 10, bool isPercent = false);

	/**
	 * @brief Sets the initial value of the slider.
	 * @details This function sets the initial value of the slider control.
	 It should be called after constructing a LabeledSlider object to set its initial value.
	 * @param[in] value The initial value to set.
	 */
	void setInitialValue(int value);

	/**
	 * @brief Returns the current value of the slider.
	 * @details This function returns the current value of the slider control.
	 It can be called at any time to get the current value selected by the user.
	 * @return An integer representing the current value of the slider.
	 */
	int value();

protected:
	/**
	 * @brief Updates the label text to reflect the current value of the slider.
	 * @details This function updates the text of the label to display the current value of the slider control.
	 If isPercent was set to true when constructing the LabeledSlider object, it also appends a percent sign to the value.
	 This function is called automatically when the value of the slider changes.
	 */
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
	/**
	 * @brief Constructs a SceneImageViewer object.
	 * @details This constructor initializes a SceneImageViewer object.
	 It creates and initializes a QGraphicsScene and QGraphicsPixmapItem.
	 These objects are used to display an image in a scrollable and zoomable view.
	 The SceneImageViewer object also keeps track of its current zoom level
	 and provides functions for zooming in, zooming out, and resetting its zoom level.
	 */
	SceneImageViewer();

	/**
	 * @brief Sets the pixmap of the image item in the scene.
	 * @details This function sets the pixmap of the QGraphicsPixmapItem in the QGraphicsScene.
	 It also adjusts various properties such as offset and scene rect to ensure that the image
	 is displayed correctly in its original size and position within the view.
	 * @param[in] pixmap The pixmap to set.
	 */
	void setPixmap(const QPixmap& pixmap);

	/**
	 * @brief Zooms in on the image by a specified number of times.
	 * @details This function zooms in on an image by scaling it up by a factor of 1.1 for each specified time.
	 It also increments its internal zoom count by that number of times and enables scroll hand drag mode so
	 that users can pan around on large images that do not fit within their viewports after being zoomed in on.
	 * @param[in] times The number of times to zoom in.
	 */
	void zoomIn(int times = 1);

	/**
	 * @brief Zooms out on an image by a specified number of times.
	 * @details This function zooms out on an image by scaling it down by a factor of 1/1.1 for each specified time.
	 It also decrements its internal zoom count by that number of times and disables scroll hand drag mode
	 if its zoom count reaches zero so that users cannot pan around on images that fit within their viewports after being zoomed out on completely.
	 * @param[in] times The number of times to zoom out.
	 */
	void zoomOut(int times = 1);

	/**
	 * @brief Resets an image's zoom level to its original state (i.e., fits it within its viewport).
	 * @details This function resets an image's zoom level by fitting it within its viewport while maintaining its aspect ratio.
	 It also sets its internal zoom count to zero and disables scroll hand drag mode so
	 that users cannot pan around on images that fit within their viewports after being reset completely.
	 */
	void zoomReset();

	/**
	 * @brief Returns an image's current zoom level (i.e., how many times it has been zoomed in or out).
	 * @details This function returns an image's current zoom level as tracked by its internal zoom count variable.
	 A positive value indicates that it has been zoomed in on while a negative value indicates that it has been zoomed out on (zero indicates no zoom).
	 * @return An integer representing an image's current zoom level (positive for zoomed in, negative for zoomed out).
	 */
	int getZoomCount();
};


class CollapsibleWidget : public QWidget {
	Q_OBJECT

private:
	QGridLayout* mainLayout;
	QToolButton* toggleButton;
	QScrollArea* contentArea;
	int collapsedHeight;
	bool isExpanded;

public slots:
	void toggle(bool collapsed);

public:
	CollapsibleWidget(const QString& title = "", QWidget* parent = 0);
	void setContentLayout(QLayout& contentLayout);
	void setTitle(QString title);
	void updateHeights();
};
