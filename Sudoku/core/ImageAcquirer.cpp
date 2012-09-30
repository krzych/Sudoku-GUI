#include "StdAfx.h"
#include "ImageAcquirer.h"

#include <opencv2\highgui\highgui.hpp>
#include <string>
#include <iostream>

ImageAcquirer::ImageAcquirer(void)
{
}


ImageAcquirer::~ImageAcquirer(void)
{
}

cv::Mat ImageAcquirer::AcquireImage(const char* file_name)
{
	cvNamedWindow("Test",CV_WINDOW_AUTOSIZE);
	CvCapture *capture = cvCreateCameraCapture(0);
	IplImage *frame;
	cv::Mat matFrame;
	while(cv::waitKey(33) != 27)
	{
		frame = cvQueryFrame(capture);
		matFrame = cv::Mat(frame,true);
		if(!frame) break;
		cvShowImage("Test",frame);
	}

	cvReleaseCapture(&capture);
	cvDestroyWindow("Test");


	return matFrame;
}