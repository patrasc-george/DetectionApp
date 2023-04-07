#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include "ObjectDetection.h"


#ifdef UI_EXPORTS
#define UI_API __declspec(dllexport)
#else
#define UI_API __declspec(dllimport)
#endif


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class UI_API MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(std::vector<Detector*>& dList, QWidget* parent = nullptr);
    ~MainWindow();
    void startVideoCapture();

private slots:
    void on_OnOff_clicked();
    void on_Exit_clicked();
    void on_detectorsList_currentTextChanged(const QString& arg1);

private:
    Ui::MainWindow* ui;
    bool onOffButtonPressed = false;
    std::vector<Detector*> detList;
    short detIndex;
};