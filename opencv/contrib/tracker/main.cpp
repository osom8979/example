#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    // declares all required variables
    Rect2d roi;
    Mat frame;

    // MIL, BOOSTING, MEDIANFLOW, TLD
    std::string tracker_string = "KCF";
    if (argc >= 2) {
        tracker_string = argv[1];
    }

    // create a tracker object
    Ptr<Tracker> tracker = Tracker::create(tracker_string);

    // set input video
    VideoCapture cap(0);

    // perform the tracking process
    printf("Start the tracking process, press ESC to quit.\n");

    for ( ;; ){
        // get frame from the video
        cap >> frame;

        // stop the program if no more images
        if(frame.rows==0 || frame.cols==0)
            break;

        if (roi.width !=0 && roi.height != 0) {
            // update the tracking result
            tracker->update(frame,roi);
            // draw the tracked object
            rectangle( frame, roi, Scalar( 255, 0, 0 ), 2, 1 );
        }

        // show image with the tracked object
        imshow("tracker",frame);

        //quit on ESC button
        int key = waitKey(1);
        if (key == 27) {
            break;
        } else if (key == (int)'c') {
            roi=selectROI("tracker",frame);
            // initialize the tracker
            tracker->init(frame,roi);
        }
    }

    return 0;
}

