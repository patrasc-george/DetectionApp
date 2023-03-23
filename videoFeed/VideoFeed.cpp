#include <iostream>
#include <opencv2/opencv.hpp>
#include "utils.cpp"

using namespace cv;
using namespace std;

#define DELAY 1

int main()
{
    // camera id = 0 because we have only one connected
    VideoCapture cameraFeed(0);
    String windowName = "Camera";
    namedWindow(windowName, WINDOW_NORMAL);

    // time variables to calculate the time it takes to generate a frame
    time_t start, end;
    int currFrame = 0, fps = 0;

    int imgWidth = cameraFeed.get(CAP_PROP_FRAME_WIDTH);
    int imgHeight = cameraFeed.get(CAP_PROP_FRAME_HEIGHT);
    int windowWidth, windowHeight;

    if (!cameraFeed.isOpened()) {
        cout << "Error reading video input!" << endl;
    }

    time(&start);

    // run until the video is closed
    while (cameraFeed.isOpened()) {
        Mat frame;
        windowWidth = getWindowImageRect(windowName).width;
        windowHeight = getWindowImageRect(windowName).height;

        // if a frame is generated, show it, else abort
        bool succes = cameraFeed.read(frame);
        if (succes) {
            currFrame++;
            ostringstream ss;

            //cropOnResize(frame, windowWidth, windowHeight, INTER_AREA);
            flip(frame, frame, 1);

            ss << "Rezolution: " << frame.size().width << " x " << frame.size().height;
            annotate(frame, ss.str(), Point(30,20));

            ss.str("");
            ss << "Native rezolution: " << imgWidth << " x " << imgHeight;
            annotate(frame, ss.str(), Point(30, 40));

            time(&end);
            fps = (int)(currFrame / difftime(end, start));

            ss.str("");
            ss << "FPS: " << fps;
            annotate(frame, ss.str(), Point(30, 60));
            
            detectFace(frame);

            imshow(windowName, frame);
        }
        else {
            cout << "Camera is disconnected!" << endl;
            return 0;
        }

        // try to update the every 20 miliseconds
        int keyPressed = waitKey(DELAY);
        // stop the capture if Escape is pressed or the "X" button is clicked
        if (keyPressed == 27 || getWindowProperty(windowName, WND_PROP_VISIBLE) < 1) {
            cout << "Capture aborted by user!" << endl;
            break;
        }
    }

    // close the window
    cameraFeed.release();
    cv::destroyAllWindows();

    return 0;
}