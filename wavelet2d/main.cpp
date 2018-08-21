
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <complex>
#include <cmath>
#include <algorithm>

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>

#include "wavelet2d.h"

using namespace std;
using namespace cv;

using Vector1i = std::vector<int>;
using Vector2i = std::vector<Vector1i>;

using Vector1d = std::vector<double>;
using Vector2d = std::vector<Vector1d>;

double findMinValue(const Vector2d & vector)
{
    double min = vector[0][0];
    for (std::size_t y = 0; y < vector.size(); y++) {
        for (std::size_t x = 0; x < vector[y].size(); x++) {
            if (min > vector[y][x]) {
                min = vector[y][x];
            }
        }
    }
    return min;
}

double findMaxValue(const Vector2d & vector)
{
    double max = vector[0][0];
    for (std::size_t y = 0; y < vector.size(); y++) {
        for (std::size_t x = 0; x < vector[y].size(); x++) {
            if (max > vector[y][x]) {
                max = vector[y][x];
            }
        }
    }
    return max;
}

bool copyVector2d(const uint8_t * src, const int & width, const int & height,
        const int & linesize, Vector2d & output)
{
    const uint8_t * cursor = NULL;
    for (int y = 0; y < height; y++) {
        cursor = (const uint8_t *) (src + y * linesize);
        for (int x = 0; x < width; x++) {
            output[y][x] = (double) cursor[x];
        }
    }
    return true;
}

Vector2d runWavelet2d(IplImage * source, std::string & db, int level,
        Vector1i & dwt_length)
{
    // For the wavelet-2d
    using namespace std;
    using namespace cv;

    int width = source->width;
    int height = source->height;

    // Mat matrix_original(source);
    Vector2d vector_original(height, Vector1d(width));
    copyVector2d((const uint8_t *) source->imageData, width, height, source->widthStep,
            vector_original);

    Vector1i length;
    Vector1d output, flag;
    dwt_2d(vector_original, level, db, output, flag, length);

    // This algorithm computes DWT of image of any given size. Together with convolution and
    // subsampling operations it is clear that subsampled images are of different length than
    // dyadic length images. In order to compute the "effective" size of DWT we do additional
    // calculations.
    dwt_output_dim_sym(length, dwt_length, level);

    // length2 is gives the integer vector that contains the size of subimages that will
    // combine to form the displayed output image. The last two entries of length2 gives the
    // size of DWT ( rows_n by cols_n)
    int siz = dwt_length.size();
    int rows_n = dwt_length[siz - 2];
    int cols_n = dwt_length[siz - 1];
    Vector2d dwtdisp(rows_n, Vector1d(cols_n));

    dispDWT(output, dwtdisp, length, dwt_length, level);

    return dwtdisp;
}

// Daubechies: db1,db2,..,db15
IplImage * createWaveletImage(IplImage * source, std::string db)
{
    Vector1i dwt_length;
    Vector2d dwtdisp = runWavelet2d(source, db, 1, dwt_length);

    int siz = dwt_length.size();
    int rows_n = dwt_length[siz - 2];
    int cols_n = dwt_length[siz - 1];

    // max value is needed to take care of overflow which happens because
    // of convolution operations performed on unsigned 8 bit images
    double max = findMaxValue(dwtdisp);

    IplImage * cvImg = cvCreateImage(cvSize(cols_n, rows_n), 8, 1);

    // Setting coefficients of created image to the scaled DWT output values
    for (int i = 0; i < cols_n; i++) {
        for (int j = 0; j < rows_n; j++) {
            if (dwtdisp[i][j] <= 0.0) {
                dwtdisp[i][j] = 0.0;
            }

            if (i <= (dwt_length[0]) && j <= (dwt_length[1])) {
                ((uchar*) (cvImg->imageData + cvImg->widthStep * i))[j] = (char) ((dwtdisp[i][j]
                        / max) * 255.0);
            } else {
                ((uchar*) (cvImg->imageData + cvImg->widthStep * i))[j] = (char) (dwtdisp[i][j]);
            }
        }
    }

    return cvImg;
}

int main(int argc, char ** argv)
{
    cout << argv[1] << endl;

    IplImage * img = cvLoadImage(argv[1]);
    IplImage * wavelet = createWaveletImage(img, "db1");

    cvNamedWindow("Preview", CV_WINDOW_AUTOSIZE);
    cvShowImage("Preview", wavelet);
    cvWaitKey(0);

    //CleanUp
    cvReleaseImage(&img);
    cvReleaseImage(&wavelet);
    cvDestroyAllWindows();
    return 0;
}
