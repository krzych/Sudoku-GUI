#include "StdAfx.h"
#include "MyMoments.h"


MyMoments::MyMoments(void)
	: nu02(0), nu03(0), nu11(0), nu12(0), nu20(0), nu21(0), nu30(0)
{
}


MyMoments::~MyMoments(void)
{
}

MyMoments::MyMoments(cv::Moments m)
{
	nu02 = m.nu02;
	nu03 = m.nu03;
	nu11 = m.nu11;
	nu12 = m.nu12;
	nu20 = m.nu20;
	nu21 = m.nu21;
	nu30 = m.nu30;
}
