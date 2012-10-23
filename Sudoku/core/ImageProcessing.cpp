#include "stdafx.h"
#include "ImageProcessing.h"
#include <boost\optional.hpp>
#include <iostream>

#include <opencv2\highgui\highgui.hpp>

using namespace std;
using namespace cv;

ImageProcessing::ImageProcessing(void)
	: ready_flag(false)
{
}


ImageProcessing::~ImageProcessing(void)
{
}

std::vector<SudokuCellImg> ImageProcessing::Run(CWnd& wnd, CString& status)
{
	assert(ready_flag == true);
	
	status = CString(_T("Extracting SUDOKU."));
	wnd.UpdateData(FALSE);
	sudoku_extractor.LoadImage(input_img);
	digit_extractor.LoadImage(sudoku_extractor.GetSudoku());


	std::vector<SudokuCellImg> v;
	for(int row = 0; row < 9; ++row) {
		for(int col = 0; col < 9; ++col) {
			status.Format(_T("Extracting cell %d out of 81."), row*9+col+1);
			wnd.UpdateData(FALSE);
			
			boost::optional<cv::Mat> digit = 
				digit_extractor.ExtractDigit(col, row, 50); //!!threshold
			if(digit) {
				SudokuCellImg cell;
				cell.row = row;
				cell.col = col;
				//cell.img = (*digit).clone();
				cell.img = *digit;
				v.push_back(cell);
			}
			cout << "Done!" << endl;
		}
	}
	return v;
}
	
void ImageProcessing::LoadImage(cv::Mat& img)
{
	input_img = img.clone();
	ready_flag = true;
}

void ImageProcessing::LoadImageSE(void)
{
	sudoku_extractor.LoadImage(input_img);
}
	
void ImageProcessing::LoadImageDE(void)
{
	LoadImageSE();
	digit_extractor.LoadImage(sudoku_extractor.GetSudoku());
}

cv::Mat ImageProcessing::GetSudoku(void)
{
	return sudoku_extractor.GetSudoku();
}
