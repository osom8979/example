#include <cassert>
#include <string>
#include <deque>
#include <iostream>
#include <sstream>

#include <opencv2/opencv.hpp>

#include <webp/encode.h>
#include <webp/types.h>
#include <webp/decode.h>

#include <cstdint>

#include <string>
#include <iostream>
#include <chrono>
#include <vector>

bool encodeAndDecode(cv::Mat const & input, cv::Mat & output)
{
    int width    = input.cols;
    int height   = input.rows;
    int channels = 3;
    int total    = width * height * channels;

    uint8_t * encode;
    auto now_time = std::chrono::system_clock::now();
    //size_t encode_size = WebPEncodeLosslessBGR(input.data(), width, height, width, &encode);
    size_t encode_size = WebPEncodeBGR(input.data, width, height, width * channels, 100, &encode);
    auto delay_time = std::chrono::system_clock::now() - now_time;
    std::cout << "Encode Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(delay_time).count() << "millisec\n";

    int decode_width = 0;
    int decode_height = 0;
    now_time = std::chrono::system_clock::now();
    uint8_t * decode_data = WebPDecodeBGR(encode, encode_size, &decode_width, &decode_height);
    output = cv::Mat(decode_height, decode_width, CV_8UC3, decode_data);
    delay_time = std::chrono::system_clock::now() - now_time;
    free(decode_data);

    std::cout << "Decode Size: " << decode_width << "x" << decode_height << ", "
              << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(delay_time).count() << "millisec\n";

    if (width != decode_width || height != decode_height) {
        std::cerr << "Size error.\n";
        return false;
    }

    return true;
}

int main(int argc, char ** argv)
{
    cv::VideoCapture cap;

    if (argc >= 2) {
        cap.open(std::string(argv[1])); // URL
    } else {
        cap.open(0);
    }

    if (cap.isOpened() == 0) {
        return 1;
    }

    cv::Mat frame;
    cv::Mat preview;

    std::string const TITLE = "Preview WebP";
    cv::namedWindow(TITLE);

    while (true) {
        cap >> frame;

        // PREVIEW CODE.
        cv::resize(frame, preview, cv::Size(300, 300));

        cv::Mat webp;
        encodeAndDecode(preview, webp);
        cv::imshow(TITLE, webp);

        int key = cv::waitKey(1);
        if (key == 'q' || key == 'Q') {
            break;
        } else if (key == 'b' || key == 'B') {
            std::cout << "Break point!";
        }
    }

    return 0;
}

