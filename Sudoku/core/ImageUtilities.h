#pragma once

#include <opencv2\core\core.hpp>
#include <vector>

namespace {
	enum { ADAPTIVE_THRESHOLD_MEAN_C, ADAPTIVE_THRESHOLD_GAUSSIAN_C};
	enum { THRESH_BINARY, THRESH_BINARY_INV};
	enum { INTER_NEAREST, INTER_LINEAR, INTER_AREA, INTER_CUBIC, INTER_LANCZOS4 }; 
};

namespace img_utilities
{

	void FindBiggestBlob(cv::Mat* img);
	void FindNBiggestBlobs(cv::Mat* img, int n);
	void Resize(const cv::Mat& input_img, cv::Mat* output_img,
		               int interpolation);
	void AdaptiveThreshold(cv::Mat* input_img, cv::Mat* output_img, 
		                          double max_val, int method, 
								  int threshold_type, 
								  int blocksize, double C);
	void GaussianBlur(cv::Mat* input_img, cv::Mat* output_img,
							 cv::Size kernel_size, double sigma);
	void Dilate(cv::Mat* input_img, cv::Mat* output_img,
					   cv::Mat& kernel);
	void Erode(cv::Mat* input_img, cv::Mat* output_img,
					   cv::Mat& kernel);
	void FloodFillBorders(cv::Mat* img, int p);
	void FloodFillColumn(cv::Mat* img, int col, cv::Scalar color);
	void FloodFillRow(cv::Mat* img, int row, cv::Scalar color);
	void GetOuterBoxVertices(const cv::Mat& img, cv::Point vertices[4]);
	 cv::Mat GetPerspectiveTransform(const cv::Point2f* src, 
										   const cv::Point2f* dst);
	void WarpPerspective(cv::Mat* input_img, cv::Mat* output_img,
							    cv::Mat& transform_matrix, cv::Size size);

	CvSeq* GetLineSequence(const cv::Mat& img, cv::Point points[4], 
		                          CvMemStorage* storage);
	bool SampleLine(const IplImage* img, cv::Point p1, cv::Point p2, 
		                   int thresh, cv::Point* point);
	void GetLineParams(const CvArr* points_seq, float* params);
	void FindSquares(const cv::Mat& img, 
		             std::vector<std::vector<cv::Point>>& squares,
					 int countourAreaThresh);
	double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);

};

