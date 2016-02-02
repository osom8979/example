
#include "test.hpp"

int test01(vector<string> & argv)
{
    cout << "OpenCV VideoCapture example.\n";

    if (argv.size() < 3) {
        cerr << "Not found url argument.";
        return 1;
    }

    string const window_title = "Preview window";
    string const video_url = argv[2]; // e.g. rtsp://root:root@192.168.1.212:554/cam0_0

    cv::VideoCapture vcap;
    cv::Mat image;

    // Open the video stream and make sure it's opened.
    if (vcap.open(video_url) == false) {
        cerr << "Error opening video stream or file.\n";
        return 1;
    }

    while (true) {
        if (vcap.read(image) == false) {
            cout << "No frame.\n";
            cv::waitKey();
        }

        cv::imshow(window_title.c_str(), image);

        if (cv::waitKey(1) >= 0) {
            break;
        }
    }

    return 0;
}

