#pragma once
#include<opencv2\opencv.hpp>

// ImageViewer


class ImageViewer : public CButton
{
	DECLARE_DYNAMIC(ImageViewer)
	cv::Mat m_img;
public:
	ImageViewer();
	virtual ~ImageViewer();
	void SetImg(const cv::Mat &img)
	{
		img.copyTo(m_img);
		Invalidate(0);
	};
protected:
	DECLARE_MESSAGE_MAP()
	
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};


