#pragma once

#include "DigitRecognizerInterface.h"
#include <opencv2\core\core.hpp>
#include <vector>
#include <string>

class SolutionViewer
{
public:
	SolutionViewer(void);
	~SolutionViewer(void);

	void LoadImage(cv::Mat& img);
	void LoadSolution(std::vector<SudokuCell> v);
	cv::Mat View(void);
	void CloseWindow(void);
	void SetWindowName(std::string name);

private:
	void WriteDigit(int col, int row, int num);

	cv::Mat input_img;
	cv::Mat output_img;
	std::vector<SudokuCell> solution;
	bool image_loaded_flag;
	bool solution_loaded_flag;
	std::string window_name;
};

