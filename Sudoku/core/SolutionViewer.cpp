#include "StdAfx.h"
#include "SolutionViewer.h"
#include <opencv2\highgui\highgui.hpp>

SolutionViewer::SolutionViewer(void)
	: image_loaded_flag(false)
	, solution_loaded_flag(false)
	, window_name("Solved SUDOKU")
{
}


SolutionViewer::~SolutionViewer(void)
{
	CloseWindow();
}

void SolutionViewer::LoadImage(cv::Mat& img)
{
	input_img = img.clone();
	image_loaded_flag = true;
}

void SolutionViewer::LoadSolution(std::vector<SudokuCell> v)
{
	solution = v;
	solution_loaded_flag = true;
}

cv::Mat SolutionViewer::View(void)
{
	assert(image_loaded_flag == true && solution_loaded_flag == true);

	output_img = input_img.clone();

	for(int i = 0; i < (int)solution.size(); ++i) {
		WriteDigit(solution[i].col, solution[i].row, solution[i].val);
	}

	//cv::imshow(window_name.c_str(), output_img);
	return output_img;
}

void SolutionViewer::CloseWindow(void)
{
	cv::destroyWindow(window_name.c_str());
}

void SolutionViewer::SetWindowName(std::string name)
{
	if(name != window_name) cv::destroyWindow(window_name.c_str());
	window_name = name;
}

void SolutionViewer::WriteDigit(int col, int row, int num)
{
	int cell_width  = (int)floor(output_img.size().width /9.0);
	int cell_height = (int)floor(output_img.size().height/9.0);
	
	int font_face = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;
	double font_scale = 1.5;
	int thickness = 3;

	cv::Point org((int)(col*cell_width+0.3*cell_width), 
		          (int)((row+1)*cell_height - 0.3*cell_height));
	char tmp[2];
	sprintf_s(tmp, "%d", num);
	putText(output_img, tmp, org, font_face, font_scale, cv::Scalar::all(0), thickness);
}