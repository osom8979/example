#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>

int main()
{
    IplImage* image = 0;
    CvCapture* capture = cvCaptureFromCAM(0);
    cvNamedWindow("T9-camera", 0);

    cvResizeWindow("T9-camera", 320, 240);

    while (1) {
        cvGrabFrame(capture);
        image = cvRetrieveFrame(capture);

        cvShowImage("T9-camera", image);

        if (cvWaitKey(10) >= 0) break;
    }

    cvReleaseCapture(&capture);
    cvDestroyWindow("T9-camera");

    return 0;
}

