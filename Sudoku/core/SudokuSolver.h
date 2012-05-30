#pragma once

#include "DigitRecognizerInterface.h"
#include <vector>

class SudokuSolver
{
public:
	typedef unsigned long BitField;
	static const unsigned block_size = 3;
	static const unsigned grid_size = block_size * block_size;
	static const BitField mask_max = 1 << grid_size;
	static const BitField all_set = mask_max - 1;
	static const unsigned invalid = grid_size + 1;

	SudokuSolver(void);
	~SudokuSolver(void);

	bool LoadInput(std::vector<SudokuCell> v);
	void Init(void);
	std::vector<SudokuCell> Run(void);

private:
	bool Solve(unsigned iMin, unsigned iMax, unsigned jMin, unsigned jMax, bool destructive, bool checkBlocks);
	inline void Set(unsigned i, unsigned j, unsigned n);
	inline unsigned GetCell(unsigned i, unsigned j);
	inline BitField BitFor(unsigned num);
	inline BitField Candidates(unsigned i, unsigned j);
	unsigned BitCount(BitField bits);
	inline void Unset(unsigned i, unsigned j, unsigned n);
	bool FindMin(unsigned iMin, unsigned iMax, unsigned jMin, unsigned jMax, unsigned &outI, unsigned &outJ);

	std::vector<std::pair<int, int>> unknows;
	BitField rows[grid_size];
	BitField cols[grid_size];
	BitField blocks[block_size][block_size];
	unsigned cells[grid_size][grid_size];
};

SudokuSolver::BitField SudokuSolver::BitFor(unsigned num) 
{ 
	return 1 << (num - 1); 
}

SudokuSolver::BitField SudokuSolver::Candidates(unsigned i, unsigned j)
{
	assert(cells[i][j] == invalid);

	return rows[i] & cols[j] & 
		blocks[i/block_size][j/block_size];
}

void SudokuSolver::Set(unsigned i, unsigned j, unsigned n)
{
	BitField bit = BitFor(n);

	// Check this is a valid value for the cell
	assert((Candidates(i, j) & bit) != 0);

	// No other cells in the row, column or block
	// can now have this value
	rows[i] &= ~bit;
	cols[j] &= ~bit;
	blocks[i/block_size][j/block_size] &= ~bit;
	cells[i][j] = n;
}

void SudokuSolver::Unset(unsigned i, unsigned j, unsigned n)
{
	BitField bit = BitFor(n);

		// Make sure it was already set
	assert(cells[i][j] == n);

	rows[i] |= bit;
	cols[j] |= bit;
	blocks[i/block_size][j/block_size] |= bit;
	cells[i][j] = invalid;
}

unsigned SudokuSolver::GetCell(unsigned i, unsigned j)
{ 
	return cells[i][j]; 
}