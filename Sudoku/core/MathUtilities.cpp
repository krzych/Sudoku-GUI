#include "StdAfx.h"
#include "MathUtilities.h"
#include <ctime>
#include <cstdlib>

bool MathUtilities::flag = false;

int MathUtilities::GetRandomNumber(int a, int b)
{
	if(flag == false) {
		srand((unsigned int)time(NULL));
		flag = true;
	}

	return (rand()%(b-a+1)) + a;
}

MathUtilities::MathUtilities(void)
{
}

MathUtilities::MathUtilities(const MathUtilities&)
{
}

MathUtilities::~MathUtilities(void)
{
}
