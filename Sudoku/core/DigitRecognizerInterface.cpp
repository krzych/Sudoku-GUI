#include "StdAfx.h"
#include "DigitRecognizerInterface.h"


DigitRecognizerInterface::DigitRecognizerInterface(void)
	: mode(DEFAULT_MODE)
	, ready_flag(false)
{
}


DigitRecognizerInterface::~DigitRecognizerInterface(void)
{
}

int DigitRecognizerInterface::FindCellInVect(int row, int col)
{
	for(int i = 0; i < (int)input_vect.size(); ++i) {
		if(input_vect[i].col == col && input_vect[i].row == row) return i;
	}
	return -1;
}