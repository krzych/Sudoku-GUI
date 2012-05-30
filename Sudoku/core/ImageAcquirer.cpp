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
	return cv::imread(file_name, CV_LOAD_IMAGE_GRAYSCALE);
}