#pragma once

#include <opencv2\imgproc\imgproc.hpp>
#include <boost\serialization\nvp.hpp>

class MyMoments
{
public:
	MyMoments(void);
	MyMoments(cv::Moments m);
	~MyMoments(void);

	double nu02;
	double nu03;
	double nu11;
	double nu12;
	double nu20;
	double nu21;
	double nu30;

private:
	friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		using boost::serialization::make_nvp;
		ar & make_nvp("nu02", nu02)
		   & make_nvp("nu03", nu03)
		   & make_nvp("nu11", nu11)
		   & make_nvp("nu12", nu12)
		   & make_nvp("nu20", nu20)
		   & make_nvp("nu21", nu21)
		   & make_nvp("nu30", nu30);
    }

};

