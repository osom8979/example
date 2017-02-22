#include <cassert>
#include <string>
#include <deque>
#include <iostream>
#include <sstream>

#include <opencv2/opencv.hpp>

int main(int argc, char ** argv)
{
    cv::VideoCapture cap;

    if (argc >= 2) {
        cap.open(std::string(argv[1]));// URL
    } else {
        cap.open(0);
    }

    if (cap.isOpened() == 0) {
        std::cerr << "Could not open the input video\n";
        return 1;
    }

    cv::Mat frame;
    cv::Mat preview;

    cap >> frame;

    cv::VideoWriter writer;
    int codec = CV_FOURCC('H', '2', '6', '4');
    double fps = 25.0;
    writer.open("live.mp4", codec, fps, frame.size(), true);

    if (writer.isOpened() == false) {
        std::cerr << "Could not open the output video file for write\n";
        return 1;
    }

    std::string const TITLE = "OpenCV Sample";
    cv::namedWindow(TITLE);

    while (true) {
        cap >> frame;

        // PREVIEW CODE.
        preview = frame;

        cv::imshow(TITLE, preview);
        writer.write(frame);

        int key = cv::waitKey(1);
        if (key == 'q' || key == 'Q') {
            break;
        } else if (key == 'b' || key == 'B') {
            std::cout << "Break point!";
        }
    }

    return 0;
}

