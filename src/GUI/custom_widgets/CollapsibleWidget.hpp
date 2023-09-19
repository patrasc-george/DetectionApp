#pragma once

#include <QWidget>
#include <QToolButton>
#include <QGridLayout>
#include <QScrollArea>

class CollapsibleWidget : public QWidget {
	Q_OBJECT

private:
	QGridLayout* mainLayout;
	QScrollArea* contentArea;
	int collapsedHeight = 0;
	bool isExpanded = false;

public:
	QToolButton* toggleButton;

public:
	/**
	* @brief A custom widget that can collapse and expand its content.
	* @details The `CollapsibleWidget` class provides a custom widget that can be collapsed and expanded
	to show or hide its content. It includes a toggle button and a scrollable content area.
	* @param[in] title The title text to be displayed on the toggle button.
	* @param[in] parent The parent widget of this `CollapsibleWidget`.
	*/
	CollapsibleWidget(const QString& title = "", QWidget* parent = 0) {
		toggleButton = new QToolButton(this);
		contentArea = new QScrollArea(this);
		mainLayout = new QGridLayout(this);

		toggleButton->setStyleSheet("QToolButton {border: none;}");
		toggleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
		toggleButton->setArrowType(Qt::ArrowType::RightArrow);
		toggleButton->setText(title);
		toggleButton->setCheckable(true);
		toggleButton->setChecked(false);

		contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

		// start out collapsed
		contentArea->setMaximumHeight(0);
		contentArea->setMinimumHeight(0);

		mainLayout->setVerticalSpacing(0);
		mainLayout->setContentsMargins(0, 0, 0, 0);

		mainLayout->addWidget(toggleButton, 0, 0, 1, 1, Qt::AlignLeft);
		mainLayout->addWidget(contentArea, 1, 0, 1, 1);
		setLayout(mainLayout);

		connect(toggleButton, &QToolButton::toggled, this, &CollapsibleWidget::toggle);
	}

	/**
	* @brief Sets the content layout for the `CollapsibleWidget`.
	* @details This function sets the content layout (the layout of the expandable content area)
	for the `CollapsibleWidget`. It replaces any existing content layout with the provided one.
	* @param[in] contentLayout The layout to be set as the content layout.
	*/
	void setContentLayout(QLayout& contentLayout) {
		delete contentArea->layout();
		contentArea->setLayout(&contentLayout);
		collapsedHeight = sizeHint().height() - contentArea->maximumHeight();
	}

	/**
	* @brief Sets the title of the `CollapsibleWidget`.
	* @param[in] title The new title to be displayed on the toggle button.
	*/
	void setTitle(QString title) {
		toggleButton->setText(std::move(title));
	}

	bool expanded() {
		return isExpanded;
	}

private slots:
	/**
	* @brief Updates the widget's height based on its expansion state.
	* @details This slot function is called when the widget's expansion state changes.
	It adjusts the widget's height to show or hide its content area accordingly.
	*/
	void updateHeights() {
		int contentHeight = contentArea->layout()->sizeHint().height();
		if (isExpanded) {
			this->setMinimumHeight(collapsedHeight + contentHeight);
			contentArea->setMaximumHeight(contentHeight);
		}
		else {
			this->setMinimumHeight(collapsedHeight);
			contentArea->setMaximumHeight(0);
		}
	}

public slots:
	/**
	 * @brief Toggles the expansion state of the `CollapsibleWidget`.
	 * @param[in] expanded True if the widget should be expanded; false if it should be collapsed.
	 */
	void toggle(bool expanded) {
		toggleButton->setArrowType(expanded ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);
		isExpanded = expanded;

		updateHeights();
	}
};
