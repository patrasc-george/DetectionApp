#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <opencv2/opencv.hpp>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QThread>

#include "ObjectDetection.h"
#include <CameraInteraction.h>
#include <QMessageBox>


MainWindow::MainWindow(std::vector<Detector*>& dList, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->detList = dList;
    this->detIndex = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startVideoCapture() {
    cv::VideoCapture cap(0);  // opens video camera
    if (!cap.isOpened())
    {
        qDebug() << "Could not open video camera.";
        return;
    }

    // create a scene to display the captured image from the webcam
    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);

    while (onOffButtonPressed && ui->graphicsView->isVisible())
    {
        float fps;
        Timer timer(fps);
        cv::Mat frame;
        if (!cap.read(frame))
        {
            break;
        }
        try {
            detList[detIndex]->detect(frame);
        }
        catch (const std::exception&) {
            QMessageBox::critical(this, "Error", "There was an error while loading the detection model");
            onOffButtonPressed = false;
            ui->OnOff->setText("Turn On");
        }
        displayInfo(frame, cv::Size(frame.cols, frame.rows), fps);


        // convert the image from OpenCV Mat format to QImage for display in QGraphicsView
        QImage qimg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
        // create a QGraphicsPixmapItem to display the image in the scene
        QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(qimg));
        // add the item to the scene and adjust the scene size to fit the item size
        scene->addItem(pixmapItem);
        scene->setSceneRect(pixmapItem->boundingRect()); // adjust the scene size to the image size
        // wait for a short interval to reduce resource consumption
        QCoreApplication::processEvents();
        QThread::usleep(1000);
    }
    cap.release();  // release the video camera resources
    delete scene;   // release the scene resources
}

void MainWindow::on_OnOff_clicked()
{
    onOffButtonPressed = !onOffButtonPressed;
    if (onOffButtonPressed)
    {
        ui->OnOff->setText("Turn Off");
    }
    else
    {
        ui->OnOff->setText("Turn On");
    }
    startVideoCapture();
}

void MainWindow::on_Exit_clicked()
{
    this->close();
}

void MainWindow::on_detectorsList_currentTextChanged(const QString& arg1) {
    if (arg1 == "Faces") {
        this->detIndex = 0;
    }
    else {
        this->detIndex = 1;
    }
}
