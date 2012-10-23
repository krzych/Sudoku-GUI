#include "StdAfx.h"
#include "DigitExtractor.h"
#include "ImageUtilities.h"
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <fstream>
#include <boost\archive\xml_oarchive.hpp>
#include <boost\archive\xml_iarchive.hpp>
#include <vector>

using namespace cv;
using boost::optional;

DigitExtractor::DigitExtractor(void)
	: ready_flag(false)
	, adjust_flag(false)
	, file_name_("DigitExtractor.xml")
{
	if(Load() == false) SetDefaultParameters();
	ClearAdjustFlags();
}

DigitExtractor::~DigitExtractor(void)
{
	Save();
}

optional<Mat> DigitExtractor::ExtractDigit(unsigned int col, 
													  unsigned int row, 
													  unsigned int thresh)
{
	assert(ready_flag == true);
	cell_img = Mat(input_img, cv::Rect(col*cell_width, row*cell_height, cell_width, cell_height));
	Preprocess(&cell_img);

	cv::Moments m = cv::moments(cell_img, true);
	if(m.m00  > cell_area/thresh) {
		static std::vector<std::vector<cv::Point>> v;
		tmp_img = cell_img.clone();
		cv::findContours(tmp_img, v, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

		int area = 0;
		int index;
		for(int i = 0; i < (int)v.size(); ++i) {
			if(area < (int)v[i].size()) index = i;
		}
		cv::Rect rect = cv::boundingRect(v[index]);
		//return boost::optional<cv::Mat>(cv::Mat(cell_img, rect).clone());
		return boost::optional<cv::Mat>(cv::Mat(cell_img, rect));
	} else {
		return boost::optional<cv::Mat>();
	}
}
	
Mat DigitExtractor::GetProcessedCell(unsigned int col, unsigned int row)
{
	if(ready_flag == false) return Mat();

	cell_img = Mat(input_img, cv::Rect(col*cell_width, row*cell_height, cell_width, cell_height)).clone();
	optional<Mat> img = Preprocess(&cell_img);
	if(img) return *img;
	else return Mat();
}

Mat DigitExtractor::GetInputImage(void)
{
	return input_img.clone();
}

void DigitExtractor::LoadImage(cv::Mat& img)
{
	input_img = img.clone();
	cell_width  = (int)floor(input_img.size().width /9.0);
	cell_height = (int)floor(input_img.size().height/9.0);
	cell_area = cell_width * cell_height; 
	ready_flag = true;
}

void DigitExtractor::ClearAdjustFlags(void)
{
	adjust_flag = false;
}

optional<Mat> DigitExtractor::Preprocess(cv::Mat* img)
{
	const unsigned int w = img->size().width;
	const unsigned int h = img->size().height;
	static Mat tmp_img = Mat(Size(5*w, h), CV_8UC1);
	static Mat roi;

	if(adjust_flag) {
		roi = Mat(tmp_img, Rect(0, 0, w, h));
		img->copyTo(roi);
	}

	img_utilities::AdaptiveThreshold(img, img, 255, ADAPTIVE_THRESHOLD_MEAN_C, cv::THRESH_BINARY_INV, block_size_, c_);
	if(adjust_flag) {
		roi = Mat(tmp_img, Rect(w, 0, w, h));
		img->copyTo(roi);
	}

	img_utilities::FloodFillBorders(img, percentage_);
	if(adjust_flag) {
		roi = Mat(tmp_img, Rect(2*w, 0, w, h));
		img->copyTo(roi);
	}

	img_utilities::FindNBiggestBlobs(img, n_);
	if(adjust_flag) {
		roi = Mat(tmp_img, Rect(3*w, 0, w, h));
		img->copyTo(roi);
	}

	Size kernel_size = Size(kernel_size_morph_, kernel_size_morph_);
	Mat kernel = getStructuringElement(MORPH_RECT, kernel_size);
	img_utilities::Dilate(img, img, kernel);
	img_utilities::FindBiggestBlob(img);
	img_utilities::Erode(img, img, kernel);
	if(adjust_flag) {
		roi = Mat(tmp_img, Rect(4*w, 0, w, h));
		img->copyTo(roi);
	}

	if(adjust_flag) return optional<Mat>(tmp_img);
	else return optional<Mat>();
}

void DigitExtractor::SetDefaultParameters(void)
{
	block_size_ = 7;
	c_ = 2;
	percentage_ = 20;
	n_ = 2;
	kernel_size_morph_ = 3;
}


bool DigitExtractor::Load(void)
{
	using boost::serialization::make_nvp;
	
	std::ifstream ifs(file_name_);
	if(ifs.is_open() == false) return false;
	boost::archive::xml_iarchive xml(ifs);
	xml >> BOOST_SERIALIZATION_NVP(*this);
	
	return true;
}
	
bool DigitExtractor::Save(void)
{
	using boost::serialization::make_nvp;

	std::ofstream ofs(file_name_);
	if(ofs.is_open() == false) return false;
	boost::archive::xml_oarchive xml(ofs);
	xml << make_nvp("params", *this);

	return true;
}