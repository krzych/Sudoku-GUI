#pragma once

#include <opencv2\core\core.hpp>

class ImageAcquirer
{
public:
	ImageAcquirer(void);
	~ImageAcquirer(void);

	cv::Mat AcquireImage(const char* file_name);
};

