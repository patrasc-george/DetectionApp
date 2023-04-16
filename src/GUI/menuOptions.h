#pragma once

#include <QWidget>
#include <QComboBox>

class Menu: public QWidget {
public:
    QPushButton* exit;
    QPushButton* toggleCamera;
    QCheckBox* toggleEyes;
    QComboBox* detectorsList;

public:
    Menu(QWidget* parent = nullptr);
};