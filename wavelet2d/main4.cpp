#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>

int main(int argc, char ** argv)
{
    char const * TITLE = "CAMERA";
    char const * WRITE_FILE = "camera.avi";
    int const FRAME_WAIT = 30;
    int const ESC_KEYCODE = 27;

    CvCapture * capture = nullptr;
    CvVideoWriter * writer = nullptr;

    if (argc >= 2) {
        capture = cvCreateFileCapture(argv[1]);
    } else {
        capture = cvCreateCameraCapture(0);
    }

    int width  = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
    int height = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
    CvSize size = { width, height };

    if (argc == 1) {
        writer = cvCreateVideoWriter(WRITE_FILE, CV_FOURCC('M','J','P','G'), FRAME_WAIT, size);
    }

    IplImage * frame = nullptr;

    cvNamedWindow(TITLE, 0);
    cvResizeWindow(TITLE, width, height);

    while (true) {
        // Get the camera frame.
        frame = cvQueryFrame(capture);

        if (!frame) {
            break;
        }

        if (writer != nullptr) {
            cvWriteFrame(writer, frame);
        }

        cvShowImage(TITLE, frame);

        if (ESC_KEYCODE == cvWaitKey(FRAME_WAIT)) {
            break;
        }
    }

    if (writer != nullptr) {
        cvReleaseVideoWriter(&writer);
    }
    cvReleaseCapture(&capture);
    cvReleaseImage(&frame);

    cvDestroyWindow(TITLE);

    return 0;
}
