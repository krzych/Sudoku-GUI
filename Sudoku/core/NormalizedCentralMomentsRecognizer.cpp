#include "StdAfx.h"
#include "NormalizedCentralMomentsRecognizer.h"
#include <fstream>
#include <boost\archive\xml_oarchive.hpp>
#include <boost\archive\xml_iarchive.hpp>
#include <opencv2\highgui\highgui.hpp>
#include "ImageUtilities.h"
#include <iostream>
#include <stdlib.h>

std::string NormalizedCentralMomentsRecognizer::filename = 
	                                                  "NrmlCentrlRcgnzr.xml";


NormalizedCentralMomentsRecognizer::NormalizedCentralMomentsRecognizer(void)
{
	if(Load() == false) {
		std::pair<int, MyMoments> p;
		p.first = 0;
		p.second = MyMoments();
		for(int i = 0; i < 9; ++i) {
			data.push_back(p);
		}
	}
}


NormalizedCentralMomentsRecognizer::~NormalizedCentralMomentsRecognizer(void)
{
	Save();
}

std::vector<SudokuCell> NormalizedCentralMomentsRecognizer::Run(void)
{
	std::vector<SudokuCell> v;
	for(int row = 0; row < 9; ++row) {
		for(int col = 0; col < 9; ++col) {
			SudokuCell cell;
			cell.row = row;
			cell.col = col;
			int index = FindCellInVect(row, col);
			if(index != -1) {
				cell.val = Recognize(input_vect[index].img);
			} 
			else {
				cell.val = 0;
			}
			v.push_back(cell);
		}
	}

	return v;
}

int NormalizedCentralMomentsRecognizer::Recognize(cv::Mat& img)
{

	MyMoments m = MyMoments(cv::moments(img, true));

	double errors[9];
	for(int i = 0; i < 9; ++i) {
		errors[i] = CalculateError(m, i);
	}

	int recognized_num =  FindMin(errors);
	if(mode == DEFAULT_MODE) {
		return recognized_num;
	}
	else if(mode == TRAINING_MODE) {
		int mag = 5;
		cv::Mat tmp = cv::Mat(
			            cv::Size(mag*img.size().width, mag*img.size().height),
                        CV_8UC1);
		img_utilities::Resize(img, &tmp, cv::INTER_LINEAR);
		cv::imshow("digit", tmp);
		int correct_num = GetUserInput();
		cv::destroyWindow("digit");
		if(correct_num != 0) Train(correct_num-1, m);
		return correct_num;
	}
	return 0;
}

void NormalizedCentralMomentsRecognizer::Train(int num, MyMoments& m)
{
	MyMoments tmp;
	int count = data[num].first;
	tmp.nu02 = ((data[num].second.nu02 * count) + m.nu02)/(count+1);
	tmp.nu03 = ((data[num].second.nu03 * count) + m.nu03)/(count+1);
	tmp.nu11 = ((data[num].second.nu11 * count) + m.nu11)/(count+1);
	tmp.nu12 = ((data[num].second.nu12 * count) + m.nu12)/(count+1);
	tmp.nu20 = ((data[num].second.nu20 * count) + m.nu20)/(count+1);
	tmp.nu21 = ((data[num].second.nu21 * count) + m.nu21)/(count+1);
	tmp.nu30 = ((data[num].second.nu30 * count) + m.nu30)/(count+1);

	data[num].first++;
	data[num].second = tmp;
}

double NormalizedCentralMomentsRecognizer::CalculateError(
	                                                 MyMoments& digit_moment, 
										             int num)
{
	int power = 2;
	double error = 0;
	
	error += pow(abs(digit_moment.nu02 - data[num].second.nu02), power);
	error += pow(abs(digit_moment.nu03 - data[num].second.nu03), power);
	error += pow(abs(digit_moment.nu11 - data[num].second.nu11), power);
	error += pow(abs(digit_moment.nu12 - data[num].second.nu12), power);
	error += pow(abs(digit_moment.nu20 - data[num].second.nu20), power);
	error += pow(abs(digit_moment.nu21 - data[num].second.nu21), power);
	error += pow(abs(digit_moment.nu30 - data[num].second.nu30), power);

	return error;
}

int NormalizedCentralMomentsRecognizer::FindMin(double errors[9])
{
	int min_index = 0;
	double min_val = errors[0];
	for(int i = 1; i < 9; ++i) {
		if(min_val > errors[i]) {
			min_index = i;
			min_val = errors[i];
		}
	}
	return min_index + 1;
}

int NormalizedCentralMomentsRecognizer::GetUserInput(void)
{
	std::cout << "What number do you see?" << std::endl;
	while(1) {
		char k = cv::waitKey(0);
		int num = atoi(&k);
		if(num >= 0 && num <= 9) return num;
	}
}

bool NormalizedCentralMomentsRecognizer::Load(void)
{
	using boost::serialization::make_nvp;
	
	std::ifstream ifs(filename);
	//assert(ifs.good());
	if(ifs.is_open() == false) return false;
	boost::archive::xml_iarchive xml(ifs);
	xml >> BOOST_SERIALIZATION_NVP(this->data);
	
	return true;
}
	
bool NormalizedCentralMomentsRecognizer::Save(void)
{
	using boost::serialization::make_nvp;

	std::ofstream ofs(filename);
	assert(ofs.good());
	boost::archive::xml_oarchive xml(ofs);
	xml << make_nvp("data", data);

	return true;
}