
#include <cassert>
#include <string>
#include <deque>
#include <iostream>
#include <sstream>

#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/math/Equation.hpp>
#include <libtbag/math/Vector.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <opencv2/opencv.hpp>

using namespace libtbag::math;

static int const INVALID_POS = -1;

enum class DrawMode
{
    NORMAL,
};

struct UserData
{
    DrawMode mode;
    std::string message;

    cv::Point2d p1;
    cv::Point2d p2;
    std::deque<cv::Point2d> check;

    int max_point = 10;

    void push(cv::Point2d const & point)
    {
        if (check.size() + 1 >= max_point) {
            check.pop_front();
        }
        check.push_back(point);
    }

    inline bool isLast2() const
    {
        return check.size() >= 2;
    }

    cv::Point2d last()
    {
        if (check.empty()) {
            return cv::Point2d();
        }
        return check.back();
    }

    cv::Point2d last2()
    {
        if (isLast2()) {
            return *(check.rbegin() + 1);
        }
        return cv::Point2d();
    }
};

void drawText(cv::InputOutputArray image, std::string const & text, int right, int top, int r, int g, int b, double scale = 1.0)
{
    int         const FONT  = cv::FONT_HERSHEY_SIMPLEX;
    int         const THICKNESS = 1;
    cv::Point2d const POINT(right, top);
    cv::Scalar  const COLOR(255, 0, 0);

    int base_line;

    cv::Size    const TEXT_SIZE = cv::getTextSize(text, FONT, scale, THICKNESS, &base_line);
    cv::Point2d const DRAW_POINT(POINT.x/*+ TEXT_SIZE.width*/, POINT.y + TEXT_SIZE.height + base_line);
    cv::putText(image, text, DRAW_POINT, FONT, scale, COLOR, THICKNESS);
}

void drawPositionText(cv::InputOutputArray image, RelativePosition pos, int right, int top, int r, int g, int b)
{
    std::string pos_text;
    switch (pos) {
    case RELATIVE_POSITION_RIGHT: pos_text = "RIGHT";   break;
    case RELATIVE_POSITION_LEFT:  pos_text = "LEFT";    break;
    case RELATIVE_POSITION_ON:    pos_text = "ON";      break;
    default:                      pos_text = "UNKNOWN"; break;
    }
    drawText(image, pos_text, right, top, r, g, b, 0.5);
}

void onClick(int event, int x, int y, int flags, void * userdata)
{
    UserData * user = static_cast<UserData*>(userdata);

    std::stringstream ss;
    ss << "Mouse x:" << x << ", y:" << y;
    user->message = ss.str();

    switch (event) {
    case cv::EVENT_LBUTTONDOWN: user->p1 = cv::Point2d(x, y);  break;
    case cv::EVENT_RBUTTONDOWN: user->p2 = cv::Point2d(x, y);  break;
    case cv::EVENT_MBUTTONDOWN: user->push(cv::Point2d(x, y)); break;
    default: break;
    }
}

struct PointInfo
{
    std::string input;

    libtbag::geometry::Pointd p11;
    libtbag::geometry::Pointd p12;
    libtbag::geometry::Pointd p21;
    libtbag::geometry::Pointd p22;

    bool is_cross = false;
};

int main(int argc, char ** argv)
{
    std::string logfile;
    if (argc >= 2) {
        logfile = argv[1];
    }

    // Format 01:
    // 10x10,10x10,10x10,10x10
    // Format 02:
    // IsCross
    std::vector<PointInfo> point_info_list;
    std::size_t point_info_index = 0;
    libtbag::filesystem::Path const LOGFILE_PATH(logfile);
    if (LOGFILE_PATH.isRegularFile()) {
        std::string content;
        if (libtbag::filesystem::readFile(LOGFILE_PATH.toString(), content) == libtbag::Err::E_SUCCESS) {
            auto list = libtbag::string::splitTokens(content, "\n");
            for (auto & cursor : list) {
                auto cols = libtbag::string::splitTokens(cursor, ",");
                PointInfo info;
                if (cols.size() == 1 && cols[0] == "IsCross") {
                    point_info_list.back().is_cross = true;
                    point_info_list.back().input += ",IsCross";
                } else {
                    info.input = cursor;
                    auto p11 = libtbag::string::splitTokens(cols[0], "x");
                    info.p11.x = std::stod(p11[0]);
                    info.p11.y = std::stod(p11[1]);

                    auto p12 = libtbag::string::splitTokens(cols[1], "x");
                    info.p12.x = std::stod(p12[0]);
                    info.p12.y = std::stod(p12[1]);

                    auto p21 = libtbag::string::splitTokens(cols[2], "x");
                    info.p21.x = std::stod(p21[0]);
                    info.p21.y = std::stod(p21[1]);

                    auto p22 = libtbag::string::splitTokens(cols[3], "x");
                    info.p22.x = std::stod(p22[0]);
                    info.p22.y = std::stod(p22[1]);
                    point_info_list.push_back(info);
                }
            }
        }
    }

    if (point_info_list.empty() == false) {
        std::cout << "Point info list size: " << point_info_list.size() << std::endl;
    }

    UserData userdata;
    userdata.mode = DrawMode::NORMAL;
    userdata.message = "Draw!";

    cv::Mat frame(900, 900, CV_8UC3);
    frame = cv::Scalar::all(0);
    cv::Mat preview;

    std::string const TITLE = "OpenCV Sample";
    cv::namedWindow(TITLE);
    cv::setMouseCallback(TITLE, onClick, &userdata);

    bool is_auto_play = false;

    while (true) {

        // PREVIEW CODE.
        frame.copyTo(preview);
        if (userdata.mode == DrawMode::NORMAL) {
            drawText(preview, userdata.message, 0, 0, 255, 0, 0);
        }

        cv::ellipse(preview, userdata.p1, cv::Size(2, 2), 0, 0, 360, cv::Scalar(0, 255, 0), CV_FILLED);
        cv::ellipse(preview, userdata.p2, cv::Size(2, 2), 0, 0, 360, cv::Scalar(0, 255, 0), CV_FILLED);
        cv::arrowedLine(preview, userdata.p1, userdata.p2, cv::Scalar(0, 126, 0));

        cv::Point2d prev(INVALID_POS, INVALID_POS);
        for (auto point : userdata.check) {
            if (prev.x != INVALID_POS && prev.y != INVALID_POS) {
                cv::Point2d cross;
                if (isCross<double, cv::Point2d>(userdata.p1, userdata.p2, prev, point, cross)) {
                    cv::line(preview, prev, point, cv::Scalar(255, 0, 0));
                    cv::ellipse(preview, cross, cv::Size(2, 2), 0, 0, 360, cv::Scalar(255, 0, 0), CV_FILLED);

                    auto pos = getRelativePositionOfPointToLine2<double, cv::Point2d>(userdata.p1, userdata.p2, point);
                    drawPositionText(preview, pos, point.x, point.y, 255, 255, 0);
                } else {
                    cv::line(preview, prev, point, cv::Scalar(0, 0, 126));
                }
            }

            prev = point;
            cv::ellipse(preview, point, cv::Size(2, 2), 0, 0, 360, cv::Scalar(0, 0, 255), CV_FILLED);
        }

        cv::imshow(TITLE, preview);

        int key = cv::waitKey(1);
        if (key == 'q' || key == 'Q') {
            break;

        } else if (key == 'p' || key == 'P') {
            if (point_info_list.empty() == false) {
                if (point_info_index > 0) {
                    point_info_index--;
                    std::cout << "Prev point: " << point_info_index
                              << " INPUT: " << point_info_list[point_info_index].input
                              << std::endl;
                }
            }

        } else if (key == 'n' || key == 'N') {
            if (point_info_list.empty() == false) {
                if (point_info_index < point_info_list.size() - 1) {
                    point_info_index++;
                    std::cout << "Next point: " << point_info_index
                              << " INPUT: " << point_info_list[point_info_index].input
                              << std::endl;
                }
            }

        } else if (key == 'b' || key == 'B') {
            std::cout << "Break point!" << std::endl;

        } else if (key == 's' || key == 'S') {
            is_auto_play = false;
            std::cout << "Stop auto-play!" << std::endl;

        } else if (key == 'a' || key == 'A') {
            is_auto_play = true;
            std::cout << "Play auto-play!" << std::endl;
        }

        if (point_info_list.empty() == false) {
            int const SCALE = 1;
            userdata.p1 = point_info_list[point_info_index].p11.toOther<cv::Point2d>() * SCALE;
            userdata.p2 = point_info_list[point_info_index].p12.toOther<cv::Point2d>() * SCALE;
            userdata.check.clear();
            userdata.check.push_back(point_info_list[point_info_index].p21.toOther<cv::Point2d>() * SCALE);
            userdata.check.push_back(point_info_list[point_info_index].p22.toOther<cv::Point2d>() * SCALE);
            //point_info_list[point_info_index].is_cross;
            cv::Point2d cross;
            auto is_cross = isCross<double, cv::Point2d>(userdata.p1, userdata.p2, userdata.check[0], userdata.check[1], cross);

            if (is_cross == false) {
                if (is_auto_play) {
                    if (point_info_index < point_info_list.size() - 1) {
                        point_info_index++;
                        std::cout << "Next point: " << point_info_index
                                  << " INPUT: " << point_info_list[point_info_index].input
                                  << std::endl;
                    }
                }
            } else {
                if (is_auto_play) {
                    std::cout << "Error point: " << point_info_index
                              << " INPUT: " << point_info_list[point_info_index].input
                              << " Current[" << is_cross << "]"
                              << std::endl;
                }
                is_auto_play = false;
            }
        }
    }

    return 0;
}

