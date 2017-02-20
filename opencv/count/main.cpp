#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

struct UserData
{
};

void onClick(int event, int x, int y, int flags, void * userdata)
{
    UserData * user = static_cast<UserData*>(userdata);

    switch (event) {
    case cv::EVENT_LBUTTONDOWN: break;
    case cv::EVENT_RBUTTONDOWN: break;
    case cv::EVENT_MBUTTONDOWN: break;
    case cv::EVENT_MOUSEMOVE:   break;
    default: break;
    }
}

int main(int argc, char ** argv)
{
    cv::VideoCapture cap(0);
    if (cap.isOpened() == 0) {
        return 1;
    }

    UserData userdata;

    cv::Mat frame;
    cv::Mat preview;

    std::string const TITLE = "OpenCV Sample";
    cv::namedWindow(TITLE);
    cv::setMouseCallback(TITLE, onClick, &userdata);

    while (true) {
        cap >> frame;

        // PREVIEW CODE.
        preview = frame;

        cv::imshow(TITLE, preview);

        int key = cv::waitKey(1);
        if (key == 'q' || key == 'Q') {
            break;
        }
    }

    return 0;
}

