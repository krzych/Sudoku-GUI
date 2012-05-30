#pragma once

#include <vector>
#include "ImageProcessing.h"

struct SudokuCell
{
	unsigned int row;
	unsigned int col;
	unsigned int val;
};

namespace 
{
	enum { DEFAULT_MODE, TRAINING_MODE };
};

class DigitRecognizerInterface
{
public:
	DigitRecognizerInterface(void);
	virtual ~DigitRecognizerInterface(void);

	virtual std::vector<SudokuCell> Run(void) = 0;
	inline void SetMode(int m);
	inline void LoadInput(std::vector<SudokuCellImg>& v);

protected:
	int FindCellInVect(int row, int col);
	int mode;
	bool ready_flag;
	std::vector<SudokuCellImg> input_vect;
};

void DigitRecognizerInterface::SetMode(int m)
{
	mode = m;
}

void DigitRecognizerInterface::LoadInput(std::vector<SudokuCellImg>& v)
{
	input_vect = v;
	ready_flag = true;
}