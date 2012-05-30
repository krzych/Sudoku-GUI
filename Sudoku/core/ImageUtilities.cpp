#include "StdAfx.h"
#include "ImageUtilities.h"
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\imgproc\imgproc_c.h>
#include <list>

using namespace std;
using namespace cv;

namespace img_utilities
{

void FindBiggestBlob(cv::Mat* img)
{
	int max_count = -1;
	Point blob_point(-1, -1);

	//search for point which belongs to biggest blob in img
	//all visited blobs are colored with color (64, 64, 64)
	for(int y = 0; y < img->size().height; ++y) {
		uchar* row = img->ptr(y);
		for(int x = 0; x < img->size().width; ++x) {
			if(row[x] > 128) {
				int count = floodFill(*img, Point(x, y), Scalar::all(64));
				if(count > max_count) {
					max_count = count;
					blob_point = Point(x, y);
				}
			}
		}
	}
	
	//color biggest blob with white (255, 255, 255)
	if(blob_point.x != -1) floodFill(*img, blob_point, Scalar::all(255));

	//color all other blobs with black (0, 0, 0)
	for(int y = 0; y < img->size().height; ++y) {
		uchar* row = img->ptr(y);
		for(int x = 0; x < img->size().width; ++x) {
			if(row[x] == 64) {
				floodFill(*img, Point(x, y), Scalar::all(0));
			}
		}
	}
}

void FindNBiggestBlobs(cv::Mat* img, int n)
{
	//for now n parameter is ignored
	
	int max_count[] = {-1, -1};
	Point blob_point[] = {Point(-1, -1), Point(-1, -1)};

	for(int y = 0; y < img->size().height; ++y) {
		uchar* row = img->ptr(y);
		for(int x = 0; x < img->size().width; ++x) {
			if(row[x] > 128) {
				
				int count = floodFill(*img, Point(x, y), Scalar::all(64));
				if(count > max_count[1]) {
					if(count > max_count[0]) {
						max_count[1] = max_count[0];
						blob_point[1] = blob_point[0];
						max_count[0] = count;
						blob_point[0] = Point(x, y);
					} else {
						max_count[1] = count;
						blob_point[1] = Point(x, y);
					}
				}

			}
		}
	}

	//color biggest blob with white (255, 255, 255)
	if(blob_point[0].x != -1) floodFill(*img, blob_point[0], Scalar::all(255));
	if(blob_point[1].x != -1) floodFill(*img, blob_point[1], Scalar::all(255));

	//color all other blobs with black (0, 0, 0)
	for(int y = 0; y < img->size().height; ++y) {
		uchar* row = img->ptr(y);
		for(int x = 0; x < img->size().width; ++x) {
			if(row[x] == 64) {
				floodFill(*img, Point(x, y), Scalar::all(0));
			}
		}
	}

}

void Resize(const cv::Mat& input_img, cv::Mat* output_img, int interpolation)
{
	cv::resize(input_img, *output_img, output_img->size(), 0, 0, interpolation);
}

void AdaptiveThreshold(cv::Mat* input_img, cv::Mat* output_img, 
		                          double max_val, int method, 
								  int threshold_type, 
								  int blocksize, double C)
{
	adaptiveThreshold(*input_img, *output_img, max_val, method, threshold_type, blocksize, C);
}

void GaussianBlur(cv::Mat* input_img, cv::Mat* output_img,
							 cv::Size kernel_size, double sigma)
{
	cv::GaussianBlur(*input_img, *output_img, kernel_size, sigma);
}

void Dilate(cv::Mat* input_img, cv::Mat* output_img,
					   cv::Mat& kernel)
{
	cv::dilate(*input_img, *output_img, kernel);
}

void Erode(cv::Mat* input_img, cv::Mat* output_img,
					   cv::Mat& kernel)
{
	cv::erode(*input_img, *output_img, kernel);
}

void FloodFillBorders(cv::Mat* img, int p)
{
	assert(p <= 100);

	int height = img->size().height;
	int width  = img->size().width;

	int num_vertical   = (int)((p/100.0) * height/2.0);
	int num_horizontal = (int)((p/100.0) * width/2.0);

	Scalar black_color = Scalar::all(0); //black
	
	//flood fill columns
	for(int x = 0; x < num_horizontal; ++x) FloodFillColumn(img, x, black_color);
	for(int x = width - num_horizontal; x < width; ++x) FloodFillColumn(img, x, black_color); 
	//flood fill rows
	for(int y = 0; y < num_vertical; ++y) FloodFillRow(img, y, black_color);
	for(int y = height - num_vertical; y < height; ++y) FloodFillRow(img, y, black_color);
}

void FloodFillColumn(cv::Mat* img, int col,	cv::Scalar color)
{
	assert(col >= 0 && col < img->size().width);
	
	for(int y = 0; y < img->size().height; ++y) {
		floodFill(*img, Point(col, y), color);
	}
}

void FloodFillRow(cv::Mat* img, int row, cv::Scalar color)
{
	assert(row >= 0 && row < img->size().height);

	for(int x = 0; x < img->size().width; ++x) {
		floodFill(*img, Point(x, row), color);
	}
}

void GetOuterBoxVertices(const cv::Mat& img, cv::Point vertices[4])
{
	CvMemStorage* storage = cvCreateMemStorage(0);
	
	int image_height = img.size().height;
	int image_width  = img.size().width;
	Point image_center = Point(image_width/2, image_height/2);
	
	int size = 50;
	Point left_upper  = Point(0                    , image_center.y - size);
	Point left_lower  = Point(0                    , image_center.y + size);
	Point right_lower = Point(image_width          , image_center.y + size);
	Point right_upper = Point(image_width          , image_center.y - size);
	Point top_left    = Point(image_center.x - size, 0);
	Point top_right   = Point(image_center.x + size, 0);
	Point down_right  = Point(image_center.x + size, image_height);
	Point down_left   = Point(image_center.x - size, image_height);

	Point points[4];
	float right_line[3];
	points[0] = right_lower; points[1] = right_upper; points[2] = left_upper; points[3] = left_lower;
	CvSeq* points_seq = GetLineSequence(img, points, storage);
	GetLineParams(points_seq, right_line);
	cvClearMemStorage(storage);

	float left_line[3];
	points[0] = left_lower; points[1] = left_upper; points[2] = right_upper; points[3] = right_lower;
	points_seq = GetLineSequence(img, points, storage);
	GetLineParams(points_seq, left_line);
	cvClearMemStorage(storage);
	
	float top_line[3];
	points[0] = top_left; points[1] = top_right; points[2] = down_right; points[3] = down_left;
	points_seq = GetLineSequence(img, points, storage);
	GetLineParams(points_seq, top_line);
	cvClearMemStorage(storage);
		
	float down_line[3];
	points[0] = down_left; points[1] = down_right; points[2] = top_right; points[3] = top_left;
	points_seq = GetLineSequence(img, points, storage);
	GetLineParams(points_seq, down_line);
	cvReleaseMemStorage(&storage);
	
	Point top_left_point;
	if(left_line[1] == 0) {
		top_left_point.x = (int)(-left_line[2]);
	} else {
		top_left_point.x = (int)((left_line[2] - top_line[2])/(top_line[0] - left_line[0]));
	}
	top_left_point.y = (int)((-top_line[2] - top_line[0]*top_left_point.x) / top_line[1]);

	Point top_right_point;
	if(right_line[1] == 0) {
		top_right_point.x = (int)(-right_line[2]);
	} else {
		top_right_point.x = (int)((right_line[2] - top_line[2])/(top_line[0] - right_line[0]));
	}
	top_right_point.y = (int)((-top_line[2] - top_line[0]*top_right_point.x) / top_line[1]);

	Point down_right_point;
	if(right_line[1] == 0) {
		down_right_point.x = (int)(-right_line[2]);
	} else {
		down_right_point.x = (int)((right_line[2] - down_line[2])/(down_line[0] - right_line[0]));
	}
	down_right_point.y = (int)((-down_line[2] - down_line[0]*down_right_point.x) / down_line[1]);

	Point down_left_point;
	if(left_line[1] == 0) {
		down_left_point.x = (int)(-left_line[2]);
	} else {
		down_left_point.x = (int)((left_line[2] - down_line[2])/(down_line[0] - left_line[0]));
	}
	down_left_point.y = (int)((-down_line[2] - down_line[0]*down_left_point.x) / down_line[1]);

	vertices[0] = top_left_point;
	vertices[1] = top_right_point;
	vertices[2] = down_right_point;
	vertices[3] = down_left_point;
}

cv::Mat GetPerspectiveTransform(const cv::Point2f* src, 
										   const cv::Point2f* dst)
{
	return getPerspectiveTransform(src, dst);
}

void WarpPerspective(cv::Mat* input_img, cv::Mat* output_img,
							    cv::Mat& transform_matrix, cv::Size size)
{
	cv::warpPerspective(*input_img, *output_img, transform_matrix, size);
}


CvSeq* GetLineSequence(const cv::Mat& img, cv::Point points[4], 
	                   CvMemStorage* storage)
{
	assert(storage != 0);

	float resolution = 300;

	float dx = (points[1].x - points[0].x)/(resolution - 1);
	float dy = (points[1].y - points[0].y)/(resolution - 1);
	
	CvSeq* seq = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), 
		                     sizeof(Point), storage);

	for(int i = 0; i < resolution; ++i) {
		Point p1 = Point((int)(points[0].x + i*dx), 
			                 (int)(points[0].y + i*dy));
		Point p2 = Point((int)(points[3].x + i*dx), 
			                 (int)(points[3].y + i*dy));
		Point p;
		if(SampleLine(&img.operator IplImage(), p1, p2, 255, &p)) {
			cvSeqPush(seq, &p);
		}
	}
	return seq;
}

bool SampleLine(const IplImage* img, cv::Point p1, cv::Point p2, 
	            int thresh, cv::Point* point)
{
	CvLineIterator it;
	int count = cvInitLineIterator(img, p1, p2, &it);
	for(int i = 0; i < count; ++i) {
		if(it.ptr[0] == thresh) {
			int y = (it.ptr - (uchar*)img->imageData)/img->widthStep;
			int x = (it.ptr - (uchar*)img->imageData -img->widthStep*y)/
				                                         img->nChannels;
			*point = Point(x, y);
			return true;
		}
		CV_NEXT_LINE_POINT(it);
	}
	return false;
}

void GetLineParams(const CvArr* points_seq, float* params)
{
	float output[4];
	cvFitLine(points_seq, CV_DIST_L2, 0, 0.0001, 0.0001, output);
	float A = output[1];
	float B = -output[0];
	Point2f line_point = Point2f(output[2], output[3]);
	//Point2D32f line_point = cvPoint2D32f(output[2], output[3]);
	float C = -A*line_point.x - B*line_point.y;

	if(abs(B) < 1e-6) B = 0;

	if(B != 0) {
		A /= B;
		C /= B;
		B /= B;
	}
	params[0] = A;
	params[1] = B;
	params[2] = C;
}

void FindSquares(const cv::Mat& img, 
	             std::vector<std::vector<cv::Point>>& squares,
				 int countourAreaThresh)
{
	const int thresh = 50;
	squares.clear();

	Mat pyr, timg, gray0(img.size(), CV_8U), gray;

	// down-scale and upscale the image to filter out the noise
    pyrDown(img, pyr, Size(img.cols/2, img.rows/2));
    pyrUp(pyr, timg, img.size());

    vector<vector<Point> > contours;
	gray0 = timg.clone();
    Canny(gray0, gray, 0, thresh, 5);

    // dilate canny output to remove potential
    // holes between edge segments
	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    dilate(gray, gray, kernel);
    // find contours and store them all as a list
    findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

    vector<Point> approx;
            
	int num = contours.size();
    // test each contour
    for( size_t i = 0; i < contours.size(); i++ ) {
        // approximate contour with accuracy proportional
        // to the contour perimeter
        approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
                
        // square contours should have 4 vertices after approximation
        // relatively large area (to filter out noisy contours)
        // and be convex.
        // Note: absolute value of an area is used because
        // area may be positive or negative - in accordance with the
        // contour orientation
        if( approx.size() == 4 &&
            fabs(contourArea(Mat(approx))) > countourAreaThresh &&
            isContourConvex(Mat(approx)) ) {
            double maxCosine = 0;

            for( int j = 2; j < 5; j++ ) {
                // find the maximum cosine of the angle between joint edges
                double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                maxCosine = MAX(maxCosine, cosine);
            }

            // if cosines of all angles are small
            // (all angles are ~90 degree) then write quandrange
            // vertices to resultant sequence
            if( maxCosine < 0.2 )
                squares.push_back(approx);
        }
	}
	//if(squares.size() == 0) return;
	for(size_t i = squares.size(); i > 0; --i) {
		Point p1 = squares[i-1][0];
		Point p2 = squares[i-1][1];
		Point p3 = squares[i-1][2];
		Point p4 = squares[i-1][3];

		double dist_p1_p2 = sqrt( pow(p1.x-p2.x, 2.0) + pow(p1.y-p2.y, 2.0) );
		double dist_p2_p3 = sqrt( pow(p2.x-p3.x, 2.0) + pow(p2.y-p3.y, 2.0) );
		double dist_p3_p4 = sqrt( pow(p3.x-p4.x, 2.0) + pow(p3.y-p4.y, 2.0) );
		double dist_p4_p1 = sqrt( pow(p4.x-p1.x, 2.0) + pow(p4.y-p1.y, 2.0) );
		double dist_mean = 0.25*(dist_p1_p2+dist_p2_p3+dist_p3_p4+dist_p4_p1);
		
		if( dist_p1_p2/dist_mean < 0.9 ||
			dist_p2_p3/dist_mean < 0.9 ||
			dist_p3_p4/dist_mean < 0.9 ||
			dist_p4_p1/dist_mean < 0.9) {
				squares.erase(squares.begin()+i-1);
		}
	}
}

double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}
}


