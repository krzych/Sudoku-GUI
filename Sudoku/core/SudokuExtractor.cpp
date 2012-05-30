#include "StdAfx.h"
#include "SudokuExtractor.h"
#include "ImageUtilities.h"
#include <vector>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <fstream>
#include <boost\archive\xml_oarchive.hpp>
#include <boost\archive\xml_iarchive.hpp>

using std::vector;
using namespace cv;

SudokuExtractor::SudokuExtractor(void)
	: extracted_flag(false)
	, ready_flag(false)
	, default_size(540, 540)
	, file_name_("SudokuExtractor.xml")
{
	if(Load() == false) SetDefaultParameters();
	ClearAdjustFlags();
}

SudokuExtractor::~SudokuExtractor(void)
{
	Save();
}

void SudokuExtractor::LoadImage(cv::Mat& img)
{
	input_img = img.clone();
	tmp_img = cv::Mat(input_img.size(), CV_8UC1);
	ready_flag = true;
}

Mat SudokuExtractor::GetInputImage(void)
{
	return input_img.clone();
}

Mat SudokuExtractor::GetProcessedImage(void)
{
	Mat img = Preprocess();
	return img.clone();
}

cv::Mat SudokuExtractor::GetSudoku(void)
{
	assert(ready_flag == true);

	if(extracted_flag == false) {
		Preprocess();
		Extract();
	}
	//return output_img.clone();
	return output_img;
}
	
cv::Mat SudokuExtractor::GetTransformationMatrix(void)
{
	assert(ready_flag == true);

	if(extracted_flag == false) {
		Preprocess();
		Extract();
	}
	return transformation_matrix.clone();
}

void SudokuExtractor::SetDefaultSize(cv::Size size)
{
	if(size.height > 50 && size.height > 50) default_size = size;
}

Mat SudokuExtractor::Preprocess(CProgressCtrl* progressBar)
{
	const unsigned int w = input_img.size().width;
	const unsigned int h = input_img.size().height;
	Mat test_img = Mat(Size(2*w, 2*h), CV_8UC1);
	Mat roi;

	Size kernel_size_gauss = Size(kernel_size_gauss_, kernel_size_gauss_);
	img_utilities::GaussianBlur(&input_img, &tmp_img, kernel_size_gauss, 0);
	if(adjust_flag) {
		progressBar->StepIt();
		roi = Mat(test_img, Rect(0, 0, w, h));
		tmp_img.copyTo(roi);
	}

	img_utilities::AdaptiveThreshold(&tmp_img, &tmp_img, 255, 
		                              ADAPTIVE_THRESHOLD_MEAN_C, 
									  cv::THRESH_BINARY_INV, block_size_, c_);
	if(adjust_flag) {
		progressBar->StepIt();
		roi = Mat(test_img, Rect(w, 0, w, h));
		tmp_img.copyTo(roi);
	}

	vector<vector<Point>> squares;
	img_utilities::FindSquares(tmp_img.clone(), squares, contour_area_thresh_);
	if(adjust_flag) {
		progressBar->StepIt();
	}
	//if(squares.size() == 0) throw "Error";

	//find square with biggest area
	int index = 0;
	double max_area = -1;
	for(size_t i = 0; i < squares.size(); ++i) {
		double area = fabs(cv::contourArea(Mat(squares[i])));
		if(area > max_area) {
			index = i;
			max_area = area;
		}
	}

	if(adjust_flag) {
		Mat tmp_img_squares = input_img.clone();
		for(int i = 0; i < squares.size(); ++i) {
			const Point* p = &squares[i][0];
			int n = (int)squares[i].size();
			polylines(tmp_img_squares, &p, &n, 1, true, cv::Scalar::all(255), 3, CV_AA);
		}
		progressBar->StepIt();
		roi = Mat(test_img, Rect(0, h, w, h));
		tmp_img_squares.copyTo(roi);
		return test_img;
	}

	Rect rect = boundingRect(squares[index]);
	offset = Point(rect.x, rect.y);
	tmp_img = Mat(tmp_img, rect).clone();

	Size kernel_size_morph = Size(kernel_size_morph_, kernel_size_morph_);
	Mat kernel = getStructuringElement(MORPH_RECT, kernel_size_morph);
	img_utilities::Dilate(&tmp_img, &tmp_img, kernel);
	img_utilities::FindBiggestBlob(&tmp_img);
	img_utilities::Erode(&tmp_img, &tmp_img, kernel);

	return Mat();
}
	
void SudokuExtractor::Extract(void)
{
	Point vertices[4];
	img_utilities::GetOuterBoxVertices(tmp_img, vertices);

	int width  = (int)(sqrt(pow((double)(vertices[0].x - vertices[1].x), 2) + 
		          pow((double)(vertices[0].y - vertices[1].y), 2)));
	int length = (int)(sqrt(pow((double)(vertices[2].x - vertices[1].x), 2) + 
		          pow((double)(vertices[2].y - vertices[1].y), 2)));

	Point2f src[4] = {vertices[0] + offset,   //top_left
		              vertices[1] + offset,   //top_right
					  vertices[2] + offset,   //down_right
					  vertices[3] + offset};  //down_left

	Point2f dst[4] = {Point2f(0, 0), 
		              Point2f(width-1.f, 0), 
				      Point2f(width-1.f, length-1.f), 
					  Point2f(0, length-1.f)};

	tmp_img = Mat(Size(width, length), CV_8UC1);
	transformation_matrix = img_utilities::GetPerspectiveTransform(src, dst);
	img_utilities::WarpPerspective(&input_img, &tmp_img, transformation_matrix, Size(width, length));

	output_img = Mat(default_size, CV_8UC1);
	img_utilities::Resize(tmp_img, &output_img, cv::INTER_LINEAR);
	//imshow("Extracted SUDOKU", output_img);
	//waitKey(100);
	extracted_flag = true;
}

void SudokuExtractor::SetDefaultParameters(void)
{
	kernel_size_gauss_ = 11;
	block_size_ = 5;
	c_ = 2;
	contour_area_thresh_ = 1000;
	kernel_size_morph_ = 3;
}

void SudokuExtractor::ClearAdjustFlags(void) 
{
	adjust_flag = false;
}

bool SudokuExtractor::Load(void)
{
	using boost::serialization::make_nvp;
	
	std::ifstream ifs(file_name_);
	if(ifs.is_open() == false) return false;
	boost::archive::xml_iarchive xml(ifs);
	xml >> BOOST_SERIALIZATION_NVP(*this);
	
	return true;
}
	
bool SudokuExtractor::Save(void)
{
	using boost::serialization::make_nvp;

	std::ofstream ofs(file_name_);
	if(ofs.is_open() == false) return false;
	boost::archive::xml_oarchive xml(ofs);
	xml << make_nvp("params", *this);

	return true;
}