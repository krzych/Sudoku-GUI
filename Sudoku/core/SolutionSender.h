#pragma once

#include "DigitRecognizerInterface.h"
#include "SolutionViewer.h"
#include <opencv2\core\core.hpp>
#include <vector>

class SolutionSender
{
public:
	SolutionSender(void);
	~SolutionSender(void);
	
	void LoadSolution(std::vector<SudokuCell> v);
	void LoadImage(cv::Mat& img);
	void Run(void);

private:
	void Send(SudokuCell cell);
	void Wait(void);
	
	SolutionViewer solution_viewer;
	std::vector<SudokuCell> solution;
	bool solution_loaded_flag;
	bool image_loaded_flag;
};

