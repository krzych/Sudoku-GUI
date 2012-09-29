#pragma once

#include "DigitRecognizerInterface.h"
#include <opencv2\core\core.hpp>
#include <boost\serialization\nvp.hpp>
#include <boost\serialization\vector.hpp>
#include <boost\serialization\utility.hpp>
#include "MyMoments.h"
#include <string>

class NormalizedCentralMomentsRecognizer :
	public DigitRecognizerInterface
{
public:
	NormalizedCentralMomentsRecognizer(void);
	virtual ~NormalizedCentralMomentsRecognizer(void);

	std::vector<SudokuCell> Run(void);

private:
	int Recognize(cv::Mat& img);
	double CalculateError(MyMoments& digit_moment, int digit_to_compare);
	int FindMin(double errors[9]);
	int GetUserInput(void);
	void Train(int num, MyMoments& m);
	bool Load(void);
	bool Save(void);

	static std::string filename;
	std::vector<std::pair<int, MyMoments>> data;

	/*friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		using boost::serialization::make_nvp;
		ar & make_nvp("data", data)
    }*/
};

