#include "StdAfx.h"
#include "SudokuSolver.h"


SudokuSolver::SudokuSolver(void)
{
	Init();
}

SudokuSolver::~SudokuSolver(void)
{
}

void SudokuSolver::Init(void)
{
	// Better initialise the arrays
	unknows.clear();
	unsigned i, j;

	for (i = 0; i < grid_size; i++) rows[i] = all_set;
	for (i = 0; i < grid_size; i++) cols[i] = all_set;

	for (i = 0; i < block_size; i++) {
		for (j = 0; j < block_size; j++) {
			blocks[i][j] = all_set;
		}
	}

	for (i = 0; i < grid_size; i++) {
		for (j = 0; j < grid_size; j++) {
			cells[i][j] = invalid;
		}
	}
}

bool SudokuSolver::LoadInput(std::vector<SudokuCell> v)
{
	for(int i = 0; i < (int)v.size(); ++i) {
		if(v[i].val != 0) {
			if((Candidates(v[i].row, v[i].col) & BitFor(v[i].val)) != 0) {
				Set(v[i].row, v[i].col, v[i].val);
			}
			else {
				Init();
				return false;
			}
		}
		else {
			std::pair<int, int> p;
			p.first = v[i].col;
			p.second = v[i].row;
			unknows.push_back(p);
		}
	}
	return true;
}

std::vector<SudokuCell> SudokuSolver::Run(void)
{
	Solve(0, 9, 0, 9, true, true);

	std::vector<SudokuCell> v;
	for(int i = 0; i < (int)unknows.size(); ++i) {
		SudokuCell cell;
		cell.col = unknows[i].first;
		cell.row = unknows[i].second;
		cell.val = GetCell(cell.row, cell.col);
		v.push_back(cell);
	}
	return v;
}

unsigned SudokuSolver::BitCount(BitField bits)
{
	unsigned result = 0;
	BitField mask = 1;

	while(mask != mask_max) {
		if (bits & mask) result++;
		mask *= 2;
	}

	return result;
}

bool SudokuSolver::FindMin(unsigned iMin, unsigned iMax, unsigned jMin, unsigned jMax, unsigned &outI, unsigned &outJ)
{
	bool found = false;
	unsigned count = 0;

	for (unsigned i = iMin; i < iMax; i++)
		for (unsigned j = jMin; j < jMax; j++)
			if (cells[i][j] == invalid &&	
				(!found || BitCount(Candidates(i, j)) < count)) {
				count = BitCount(Candidates(i, j));
				outI = i;
				outJ = j;
				found = true;
			}

	return found;
}

bool SudokuSolver::Solve(unsigned iMin, unsigned iMax, 
		       unsigned jMin, unsigned jMax, bool destructive, bool checkBlocks)
{
	// Check that each block can be filled if we've been asked to
	if (checkBlocks) {
		for (unsigned i = 0; i < block_size; i++)
			for (unsigned j = 0; j < block_size; j++)
				if (!Solve( i*block_size, i*block_size + block_size,
					j*block_size, j*block_size + block_size, 
					false, false))
						return false;
	}

	// Guess a good cell to brute-force with
	unsigned i;
	unsigned j;

	if (!FindMin(iMin, iMax, jMin, jMax, i, j))
		// We must have finished
		return true;

	// Iterate through the possible values this cell could have
	unsigned num = 1;
	unsigned mask = BitFor(num);

	while(mask != mask_max) {
		if (Candidates(i, j) & mask) {

			// Try this number
			Set(i, j, num);

			bool solved = (Solve(iMin, iMax, jMin, jMax, 
				destructive, checkBlocks));

			// Reverse the changes if needed
			if (!solved || !destructive)
				Unset(i, j, num);

			if (solved)
				return true;

		}
			

		// Advance to the next number
		mask *= 2;
		num++;
	}

	// None of the possibilities for cell (i,j) work
	return false;
}