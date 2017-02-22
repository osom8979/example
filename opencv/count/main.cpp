#include <cassert>
#include <string>
#include <deque>
#include <iostream>
#include <sstream>

#include <opencv2/opencv.hpp>

constexpr int INVALID_POS = -1;

template <typename T>
struct LinearEquation
{
    T a; ///< Gradient.
    T b; ///< Y-intercept.
};

template <typename T>
inline T getY(LinearEquation<T> e, T x)
{
    return e.a * x + e.b;
}

template <typename T>
inline LinearEquation<T> getLinearEquationWithTwoPoint(T x1, T y1, T x2, T y2)
{
    assert(x1 != x2);
    LinearEquation<T> result;
    result.a = (y2 - y1) / (x2 - x1);
    result.b = result.a * (-1) * x1 + y1;
    return result;
}

template <typename T>
inline LinearEquation<T> getLinearEquationWithTwoPoint(cv::Point_<T> const & p1, cv::Point_<T> const & p2)
{
    return getLinearEquationWithTwoPoint(p1.x, p1.y, p2.x, p2.y);
}

template <typename T>
inline bool isParallelWithTwoLinearEquation(LinearEquation<T> const & e1, LinearEquation<T> const & e2)
{
    return e1.a == e2.a;
}

template <typename T>
inline cv::Point_<T> getIntersectionWithTwoLinearEquation(LinearEquation<T> const & e1, LinearEquation<T> const & e2)
{
    assert(e1.a != e2.a);
    cv::Point_<T> result;
    result.x = (e2.b - e1.b) / (e1.a - e2.a);
    result.y = e1.a * result.x + e1.b;
    return result;
}

template <typename T>
bool isContains(T const & p1, T const & p2, T const & check)
{
    T const min = std::min(p1, p2);
    T const max = std::max(p1, p2);
    return (min <= check && check <= max);
}

template <typename T, typename Point = cv::Point_<T> >
bool isContains2d(Point const & p1, Point const & p2, Point const & check)
{
    T const x_min = std::min(p1.x, p2.x);
    T const x_max = std::max(p1.x, p2.x);
    T const y_min = std::min(p1.y, p2.y);
    T const y_max = std::max(p1.y, p2.y);
    return (x_min <= check.x && check.x <= x_max &&
            y_min <= check.y && check.y <= y_max);
}

template <typename T, typename Point = cv::Point_<T> >
bool isCross(Point const & p11, Point const & p12, Point const & p21, Point const & p22, Point & cross)
{
    if (p11.x != p12.x && p21.x != p22.x) {
        LinearEquation<T> e1 = getLinearEquationWithTwoPoint<T>(p11, p12);
        LinearEquation<T> e2 = getLinearEquationWithTwoPoint<T>(p21, p22);

        if (isParallelWithTwoLinearEquation<T>(e1, e2) == false) {
            cross = getIntersectionWithTwoLinearEquation(e1, e2);
            return isContains2d<T, Point>(p11, p12, cross) && isContains2d<T, Point>(p21, p22, cross);
        } else {
            return false;
        }

    } else if (p11.x != p12.x && p21.x == p22.x) {
        LinearEquation<T> e1 = getLinearEquationWithTwoPoint<T>(p11, p12);
        Point cross = Point(p21.x, getY(e1, p21.x));
        return isContains<T>(p21.y, p22.y, cross.y) && isContains2d<T, Point>(p11, p12, cross);

    } else if (p11.x == p12.x && p21.x != p22.x) {
        LinearEquation<T> e2 = getLinearEquationWithTwoPoint<T>(p21, p22);
        Point cross = Point(p11.x, getY(e2, p11.x));
        return isContains<T>(p11.y, p12.y, cross.y) &&  isContains2d<T, Point>(p21, p22, cross);
    }
    return false;
}

/**
 * @return If a positive number, the right side of the vector. @n
 *         if a negative number, the left side of the vector. @n
 *         if 0 is on the line.
 */
template <typename T, typename Point = cv::Point_<T> >
inline T getRelativePositionOfPointToLine(Point const & p0, Point const & p1, Point const & check)
{
    return ((p1.x - p0.x) * (check.y - p0.y)) - ((p1.y - p0.y) * (check.x - p0.x));
}

enum class RelativePosition
{
    RIGHT,
    LEFT,
    ON
};

template <typename T, typename Point = cv::Point_<T> >
inline RelativePosition getRelativePositionOfPointToLine2(Point const & p0, Point const & p1, Point const & check)
{
    T const RESULT = getRelativePositionOfPointToLine<T, Point>(p0, p1, check);
    if (RESULT > 0) {
        return RelativePosition::RIGHT;
    } else if (RESULT < 0) {
        return RelativePosition::LEFT;
    } else {
        return RelativePosition::ON;
    }
}

enum class DrawMode
{
    NORMAL,
};

struct UserData
{
    DrawMode mode;
    std::string message;

    cv::Point p1;
    cv::Point p2;
    std::deque<cv::Point> check;

    int max_point = 10;

    void push(cv::Point const & point)
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

    cv::Point last()
    {
        if (check.empty()) {
            return cv::Point();
        }
        return check.back();
    }

    cv::Point last2()
    {
        if (isLast2()) {
            return *(check.rbegin() + 1);
        }
        return cv::Point();
    }
};

void drawText(cv::InputOutputArray image, std::string const & text, int right, int top, int r, int g, int b, double scale = 1.0)
{
    int        const FONT  = cv::FONT_HERSHEY_SIMPLEX;
    int        const THICKNESS = 1;
    cv::Point  const POINT(right, top);
    cv::Scalar const COLOR(255, 0, 0);

    int base_line;

    cv::Size  const TEXT_SIZE = cv::getTextSize(text, FONT, scale, THICKNESS, &base_line);
    cv::Point const DRAW_POINT(POINT.x/*+ TEXT_SIZE.width*/, POINT.y + TEXT_SIZE.height + base_line);
    cv::putText(image, text, DRAW_POINT, FONT, scale, COLOR, THICKNESS);
}

void drawPositionText(cv::InputOutputArray image, RelativePosition pos, int right, int top, int r, int g, int b)
{
    std::string pos_text;
    switch (pos) {
    case RelativePosition::RIGHT: pos_text = "RIGHT";   break;
    case RelativePosition::LEFT:  pos_text = "LEFT";    break;
    case RelativePosition::ON:    pos_text = "ON";      break;
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
    case cv::EVENT_LBUTTONDOWN: user->p1 = cv::Point(x, y);  break;
    case cv::EVENT_RBUTTONDOWN: user->p2 = cv::Point(x, y);  break;
    case cv::EVENT_MBUTTONDOWN: user->push(cv::Point(x, y)); break;
    default: break;
    }
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

    UserData userdata;
    userdata.mode = DrawMode::NORMAL;
    userdata.message = "Draw!";

    cv::Mat frame;
    cv::Mat preview;

    std::string const TITLE = "OpenCV Sample";
    cv::namedWindow(TITLE);
    cv::setMouseCallback(TITLE, onClick, &userdata);

    while (true) {
        cap >> frame;

        // PREVIEW CODE.
        preview = frame;
        if (userdata.mode == DrawMode::NORMAL) {
            drawText(preview, userdata.message, 0, 0, 255, 0, 0);
        }

        cv::ellipse(preview, userdata.p1, cv::Size(2, 2), 0, 0, 360, cv::Scalar(0, 255, 0), CV_FILLED);
        cv::ellipse(preview, userdata.p2, cv::Size(2, 2), 0, 0, 360, cv::Scalar(0, 255, 0), CV_FILLED);
        cv::arrowedLine(preview, userdata.p1, userdata.p2, cv::Scalar(0, 126, 0));

        cv::Point prev(INVALID_POS, INVALID_POS);
        for (auto point : userdata.check) {
            if (prev.x != -1 || prev.y != -1) {
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
        } else if (key == 'b' || key == 'B') {
            std::cout << "Break point!";
        }
    }

    return 0;
}

