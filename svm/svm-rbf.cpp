#include "stdafx.h"
#include <opencv2/opencv.hpp>

static const int MAX_CLASS = 4;	// 클래스 수

static struct sSampleParam {
	CvScalar color_pt;			// 셈플 색
	CvScalar color_bg;			// 배경 색
} sample_param[MAX_CLASS] = {
	{ CV_RGB(180, 0, 0), CV_RGB(255, 0, 0), },
	{ CV_RGB(0, 180, 0), CV_RGB(0, 255, 0), },
	{ CV_RGB(0, 0, 180), CV_RGB(0, 0, 255), },
	{ CV_RGB(180, 0, 180), CV_RGB(255, 0, 255), },
};


void SVM_RBF ()
{
	cv::RNG rng;

	IplImage *img = cvCreateImage (cvSize (1000, 1000), IPL_DEPTH_8U, 3);
	cvZero (img);

	cvNamedWindow ("result", CV_WINDOW_AUTOSIZE);

	// 학습 데이터의 총 수
	int sample_count = 1000;

	// 학습 데이터와 클래스를 할당할 행렬 생성
	CvMat *train_data  = cvCreateMat (sample_count, 2, CV_32FC1);
	CvMat *train_class = cvCreateMat (sample_count, 1, CV_32SC1);
	
	// 무작위로 학습 데이터를 생성 후 영역 별로 클래스를 할당
	for (int k=0; k<sample_count; k++){
		float x = (float)rng.uniform (0, img->width);
		float y = (float)rng.uniform (0, img->height);
		
		// !!! 학습 데이터는 0~1 사이로 정규화 하여 사용한다.
		CV_MAT_ELEM(*train_data, float,k,0) = x/img->width;
		CV_MAT_ELEM(*train_data, float,k,1) = y/img->height;

		int c = (y > 200*cos(x*CV_PI/300) + 400) ? ((x > 600) ? 0 : 1) : ((x > 400) ? 2 : 3);
		CV_MAT_ELEM(*train_class,long,k,0) = c;
	}
	
	// SVM 학습 매개 변수를 다음과 같이 설정한다.
	// svm 유형  : CvSVM::C_SVC 
	// 커널 종류 : CvSVM::RBF 
	// degree    : 10.0 (사용안함) 
	// gamma     : 8.0 
	// coef0	 : 1.0 (사용안함) 
	// C         : 10.0 
	// nu        : 0.5 (사용안함) 
	// p         : 0.1 (사용안함)
	CvTermCriteria criteria = cvTermCriteria (CV_TERMCRIT_EPS, 100, 1e-6);
	CvSVMParams param (CvSVM::C_SVC, CvSVM::RBF, 10.0, 8.0, 1.0, 10.0, 0.5, 0.1, NULL, criteria);
	
	CvSVM svm;

	// SVM 분류기가 주어진 데이터와 클래스를 학습한다.
	svm.train(train_data, train_class, cv::Mat(), cv::Mat(), param);
	
	// SVM 분류기가 이미지의 모든 픽셀에 대해 각 픽셀이 
	// 어느 클래스에 속하는지 추정하여 클래스를 할당한다.
	for (int x=0; x<img->width; x++) {
		for (int y=0; y<img->height; y++) {
			float sample_[2] = {(float)x/img->width, (float)y/img->height};
			CvMat sample = cvMat (1, 2, CV_32FC1, sample_);

			// SVM 분류기가 주어진 픽셀이 어느 클래스에 속하는지 추정한다.
			float response = svm.predict (&sample);

			// 이미지에 추정된 클래스를 색으로 표시한다.
			cvSet2D (img, y, x, sample_param[cvRound(response)].color_pt); 
		}
	}

	// 학습 데이터를 이미지에 그린다.
	for (int k=0; k<sample_count; k++) {
		int x = cvRound(img->width* CV_MAT_ELEM(*train_data, float,k,0));
		int y = cvRound(img->height*CV_MAT_ELEM(*train_data, float,k,1));
		int c = cvRound(CV_MAT_ELEM(*train_class,long, k,0));

		cvCircle (img, cvPoint(x, y), 2, sample_param[c].color_bg, CV_FILLED);
	}

	// 서포트 벡터를 그린다.
	int n = svm.get_support_vector_count ();
	for (int i=0; i<n; i++) {
		const float *support = svm.get_support_vector (i);
		int x = cvRound(support[0]*img->width);
		int y = cvRound(support[1]*img->height);

		cvCircle (img, cvPoint (x, y), 4, CV_RGB (255, 255, 255), 1);
	}

	cvShowImage ("result", img);
	
	// 키를 누르면 종료
	cvWaitKey (0);

	cvReleaseMat (&train_class);
	cvReleaseMat (&train_data);

	cvDestroyWindow ("result");
	cvReleaseImage (&img);
}

int main()
{
	SVM_RBF ();

	return 0;
}
