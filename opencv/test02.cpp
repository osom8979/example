
#include "test.hpp"

int test02(vector<string> & argv)
{
    std::string const WINDOW_TITLE = "CAMERA";

    IplImage  * image = nullptr;
    CvCapture * capture = cvCaptureFromCAM(0);

    cvNamedWindow(WINDOW_TITLE.c_str(), 0);
    cvResizeWindow(WINDOW_TITLE.c_str(), 320, 240);

    while (true) {
        cvGrabFrame(capture);
        image = cvRetrieveFrame(capture);

        cvShowImage(WINDOW_TITLE.c_str(), image);

        if (cvWaitKey(10) >= 0) { // Any key.
            break;
        }
    }

    cvReleaseCapture(&capture);
    cvDestroyWindow(WINDOW_TITLE.c_str());

    return 0;
}

