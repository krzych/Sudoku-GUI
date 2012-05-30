#include "StdAfx.h"
#include "SolutionSender.h"
#include "MathUtilities.h"
#include <opencv2\highgui\highgui.hpp>

SolutionSender::SolutionSender(void)
	: solution_loaded_flag(false)
	, image_loaded_flag(false)
{
}


SolutionSender::~SolutionSender(void)
{
}

void SolutionSender::LoadSolution(std::vector<SudokuCell> v)
{
	solution = v;
	solution_loaded_flag = true;
}

void SolutionSender::LoadImage(cv::Mat& img)
{
	solution_viewer.LoadImage(img);
	image_loaded_flag = true;
}

void SolutionSender::Run(void)
{
	assert(solution_loaded_flag == true);

	std::vector<SudokuCell> v;
	while(solution.size() != 0) {
		int num = MathUtilities::GetRandomNumber(0, solution.size() - 1);
		Send(solution[num]);
		
		v.push_back(solution[num]);
		solution.erase(solution.begin()+num);

		if(image_loaded_flag == true) {
			solution_viewer.LoadSolution(v);
			solution_viewer.View();
		}

		Wait();
	}
}

void SolutionSender::Send(SudokuCell cell)
{
	
}

void SolutionSender::Wait(void)
{
	int delay = MathUtilities::GetRandomNumber(100, 300);
	cv::waitKey(delay);
}