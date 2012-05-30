#pragma once

#include <opencv2\core\core.hpp>
#include <boost\serialization\nvp.hpp>
#include "afxwin.h"
#include <afxcmn.h>

using namespace cv;

class SudokuExtractor
{
public:
	SudokuExtractor(void);
	~SudokuExtractor(void);

	void LoadImage(cv::Mat& img);
	Mat GetInputImage(void);
	Mat GetProcessedImage(void);
	Mat GetSudoku(void);
	Mat GetTransformationMatrix(void);
	void SetDefaultSize(Size size);

	inline int GetKernelSizeGauss(void) { return kernel_size_gauss_; }
	inline void SetKernelSizeGauss(int kernel_size) { kernel_size_gauss_ = kernel_size; }
	inline int GetBlockSize(void) { return block_size_; }
	inline void SetBlockSize(int blockSize) { block_size_ = blockSize; }
	inline double GetC(void) { return c_; }
	inline void SetC(double c) { c_ = c; }
	inline int GetContourAreaThresh(void) { return contour_area_thresh_; }
	inline void SetContourAreaThresh(int contour_area_thresh) { contour_area_thresh_ = contour_area_thresh; }
	inline int GetKernelSizeMorph(void) { return kernel_size_morph_; }
	inline void SetKernelSizeMorph(int kernel_size) { kernel_size_morph_ = kernel_size; }
	
	void ClearAdjustFlags(void);
	inline void SetAdjustFlag(void) { adjust_flag = true; }
	Mat Preprocess(CProgressCtrl* progressBar = NULL);

private:
	void Extract(void);
	void SetDefaultParameters(void);
	bool Load(void);
	bool Save(void);

	Mat input_img;
	Mat tmp_img;
	Mat output_img;
	Mat transformation_matrix;
	Point offset;
	Size default_size;
	bool extracted_flag;
	bool ready_flag;
	bool adjust_flag;

	//parameters
	std::string file_name_;
	int kernel_size_gauss_;
    int block_size_;
    double c_;
    int contour_area_thresh_;
    int kernel_size_morph_;

	friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		using boost::serialization::make_nvp;
		ar & make_nvp("kernel_size_gauss", kernel_size_gauss_)
		   & make_nvp("block_size", block_size_)
		   & make_nvp("c", c_)
		   & make_nvp("contour_area_thresh", contour_area_thresh_)
		   & make_nvp("kernel_size_morph", kernel_size_morph_);
    }
};

