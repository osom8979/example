#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv/cvaux.h>

int main(int argc, char ** argv)
{
    cv::VideoCapture cap(0);
    if (cap.isOpened() == 0) {
        return 1;
    }

    cv::Mat frame;
    cv::Mat preview;

    int    const history        = 500;
    double const threshold      = 8.0f * 4.0f;
    bool   const detect_shadows = false;
    double const learning_rate = -1;

    cv::Ptr<cv::BackgroundSubtractorMOG2> model = cv::createBackgroundSubtractorMOG2(history, threshold, detect_shadows);
    cv::Mat mask;

    std::string const TITLE = "OpenCV Sample";
    cv::namedWindow(TITLE);


    std::vector<std::vector<cv::Point> > contours;
    while (true) {
        cap >> frame;

        // PREVIEW CODE.
        model->apply(frame, mask, learning_rate);
        preview = mask;

        // SegmentFGMask.
        // cv::Mat segmented_mask;
        // cv::SegmentFGMask(mask,segmented_mask,true,16.0f);
        // cv::Mat tmp;
        // cv::hconcat(mask,segmented_mask,tmp);
        // preview = tmp;

        //cv::findContours(mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        //cv::Mat result(mask.size(), CV_8U, cv::Scalar(255));
        //cv::drawContours(result, contours, -1, cv::Scalar(0), 2);
        // preview = result;

        cv::imshow(TITLE, preview);
        if (cv::waitKey(1) >= 0) {
            break;
        }
    }

    return 0;
}

