#pragma once

class MathUtilities
{
public:
	static int GetRandomNumber(int a, int b);
	
private:
	MathUtilities(void);
	MathUtilities(const MathUtilities&);
	~MathUtilities(void);

	static bool flag;
};

