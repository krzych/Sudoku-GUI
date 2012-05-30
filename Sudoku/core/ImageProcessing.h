#pragma once

#include "SudokuExtractor.h"
#include "DigitExtractor.h"
#include <opencv2\core\core.hpp>
#include <vector>
#include <Windows.h>
#include "afxwin.h"

struct SudokuCellImg
{
	unsigned int row;
	unsigned int col;
	cv::Mat img;
};

class ImageProcessing
{
public:
	ImageProcessing(void);
	~ImageProcessing(void);

	std::vector<SudokuCellImg> Run(CWnd& wnd, CString& status);
	void LoadImage(cv::Mat& img);
	void LoadImageSE(void);
	void LoadImageDE(void);
	cv::Mat GetSudoku(void);

	SudokuExtractor sudoku_extractor;
	DigitExtractor digit_extractor;

private:
	cv::Mat input_img;
	bool ready_flag;
};

