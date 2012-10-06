#include "StdAfx.h"
#include "ImageAcquirer.h"

#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <string>
#include <iostream>

#define ANDROIDCAM

#ifdef ANDROIDCAM
#define SHOTURL "http://192.168.1.104:8080/shot.jpg"
#define TEMPJPG "D:/shot.jpg"
#include <UrlMon.h>
#pragma comment(lib,"urlmon.lib")
#endif

ImageAcquirer::ImageAcquirer(void)
{
}


ImageAcquirer::~ImageAcquirer(void)
{
}

cv::Mat ImageAcquirer::AcquireImage(const char* file_name)
{
	cvNamedWindow("Video",CV_WINDOW_AUTOSIZE);
	cv::Mat matFrame;
#ifdef ANDROIDCAM
	while(cv::waitKey(5) != 27)
	{
		HRESULT hr = URLDownloadToFile(NULL, _T(SHOTURL),
			_T(TEMPJPG),0,NULL);
		IplImage *frame = cvLoadImage(TEMPJPG);
		matFrame = cv::Mat(frame,true);
		cvShowImage("Video",frame);
	}
#else
	CvCapture *capture = cvCreateCameraCapture(0);
	IplImage *frame;
	
	while(cv::waitKey(33) != 27)
	{
		frame = cvQueryFrame(capture);
		matFrame = cv::Mat(frame,true);
		if(!frame) break;
		cvShowImage("Video",frame);
	}

	cvReleaseCapture(&capture);
#endif
	cv::cvtColor(matFrame,matFrame,CV_BGR2GRAY);
	cvDestroyWindow("Video");

	return matFrame;
}