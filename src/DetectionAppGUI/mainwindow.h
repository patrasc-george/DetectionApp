
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void startVideoCapture();

private slots:
    void on_OnOff_clicked();
    void on_Exit_clicked();

private:
    Ui::MainWindow *ui;
    bool onOffButtonPressed=false;
};

#endif // MAINWINDOW_H
