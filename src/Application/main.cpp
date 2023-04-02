#include <iostream>
#include <chrono>
#include <opencv2/opencv.hpp>

#include "CameraInteraction.h"
#include "ObjectDetection.h"

using namespace std;
using namespace cv;

#define DELAY 1
#define WINDOW_NAME "Camera"


int main() {
    // Load cascade classifier
    CascadeClassifier cs;
    cs.load((string(getenv("OpenCV_DIR")) + "/etc/haarcascades/haarcascade_frontalface_alt.xml"));

    // Open default camera
    VideoCapture cameraFeed(0);
    namedWindow(WINDOW_NAME, WINDOW_AUTOSIZE);

    // Check if camera opened successfully
    if (!cameraFeed.isOpened()) {
        cerr << "Error opening camera" << endl;
        return -1;
    }

    // Start timer
    chrono::time_point<chrono::system_clock> startTime = chrono::system_clock::now();

    Mat frame;
    // Capture and process frames from camera
    while (cameraFeed.isOpened()) {
        // Capture frame
        bool succes = cameraFeed.read(frame);

        // Check if frame is empty
        if (frame.empty() || !succes) {
            cerr << "Camera is disconnected" << endl;
            return -1;
        }

        // Detect objects in frame
        vector<Rect> objects;
        try {
            detectObjects(frame, cs, objects);
        }
        catch (const std::exception&) {
            putText(frame, "Error loading classifier", Point(100, 200), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 1);
        }

        // Calculate FPS
        float fps;
        calculateFps(fps, startTime);

        Size nativeRes(frame.cols, frame.rows);

        /*
        // Crop the frame to fit window
        //Size res(getWindowImageRect(WINDOW_NAME).width, getWindowImageRect(WINDOW_NAME).height);
        int winWidth = getWindowImageRect(WINDOW_NAME).width;
        int winHeight = getWindowImageRect(WINDOW_NAME).height;
        //cropOnResize(frame, res, INTER_AREA);

        //TODO: optimise crop function
        cropOnResize(frame, winWidth, winHeight, INTER_LINEAR);
        */

        // Display frame and info
        displayInfo(frame, nativeRes, fps);
        imshow(WINDOW_NAME, frame);

        // Check for user exit command
        if (waitKey(DELAY) == 27 || getWindowProperty(WINDOW_NAME, WND_PROP_VISIBLE) < 1) {
            cout << "Capture aborted by user!" << endl;
            break;
        }
    }

    // Release camera and destroy windows
    cameraFeed.release();
    cv::destroyAllWindows();

    return 0;
}