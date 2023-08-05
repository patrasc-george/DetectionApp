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
	CollapsibleWidget(const QString& title = "", QWidget* parent = 0){
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

    void setContentLayout(QLayout& contentLayout) {
        delete contentArea->layout();
        contentArea->setLayout(&contentLayout);
        collapsedHeight = sizeHint().height() - contentArea->maximumHeight();
    }

    void setTitle(QString title) {
        toggleButton->setText(std::move(title));
    }

private slots:
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
    void toggle(bool expanded) {
        toggleButton->setArrowType(expanded ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);
        isExpanded = expanded;

        updateHeights();
    }
};
