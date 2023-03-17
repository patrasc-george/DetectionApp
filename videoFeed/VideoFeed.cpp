#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    // camera id = 0 because we have only one connected
    VideoCapture cameraFeed(0);
    String windowName = "Camera";
    namedWindow(windowName, WINDOW_NORMAL);

    if (!cameraFeed.isOpened()) {
        cout << "Error reading video input!" << endl;
    }

    // run until the video is closed
    while (cameraFeed.isOpened()) {
        Mat frame;
        float windowWidth = getWindowImageRect(windowName).width;
        float windowHeight = getWindowImageRect(windowName).height;

        float imgWidth = cameraFeed.get(CAP_PROP_FRAME_WIDTH);
        float imgHeight = cameraFeed.get(CAP_PROP_FRAME_HEIGHT);

        // if a frame is generated, show it, else abort
        bool succes = cameraFeed.read(frame);
        if (succes) {

            flip(frame, frame, 1);

            Mat resized_img;
            float scale = windowWidth / imgWidth;
            resize(frame, resized_img, Size(), scale, scale, INTER_AREA);

            Mat crop;
            if (resized_img.size().height > windowHeight) {
                crop = resized_img(Range((resized_img.size().height - windowHeight)/2, windowHeight + (resized_img.size().height - windowHeight) / 2),
                    Range(0, resized_img.size().width));
            }
            else {
                scale = windowHeight / imgHeight;
                resize(frame, resized_img, Size(), scale, scale, INTER_AREA);
                crop = resized_img(Range(0, resized_img.size().height),
                    Range((resized_img.size().width - windowWidth) / 2, windowWidth + (resized_img.size().width - windowWidth) / 2));
            }
                imshow(windowName, crop);
        }
        else {
            cout << "Camera is disconnected!" << endl;
            return 0;
        }

        // try to update the every 20 miliseconds
        int keyPressed = waitKey(1000/120);
        // stop the capture if Escape is pressed or the "X" button is clicked
        if (keyPressed == 27 || getWindowProperty(windowName, WND_PROP_VISIBLE) < 1) {
            cout << "Capture aborted by user!" << endl;
            break;
        }

    }

    // close the window
    cameraFeed.release();
    destroyAllWindows();

    return 0;
}