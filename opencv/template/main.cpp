#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

int main(int argc, char ** argv)
{
    cv::VideoCapture cap(0);
    if (cap.isOpened() == 0) {
        return 1;
    }

    cv::Mat frame;
    cv::Mat preview;

    std::string const TITLE = "OpenCV Sample";
    cv::namedWindow(TITLE);

    while (true) {
        cap >> frame;

        // PREVIEW CODE.
        preview = frame;

        cv::imshow(TITLE, preview);
        if (cv::waitKey(1) >= 0) {
            break;
        }
    }

    return 0;
}

