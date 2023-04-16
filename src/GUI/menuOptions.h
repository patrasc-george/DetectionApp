#pragma once

#include <QWidget>
#include <QComboBox>

class Menu: public QWidget {
public:
    QPushButton* exit;
    QPushButton* toggleCamera;
    QCheckBox* toggleEyes;
    QCheckBox* showRes;
    QCheckBox* showFps;
    QComboBox* detectorsList;
    QPushButton* screenshot;

public:
    Menu(QWidget* parent = nullptr);
};