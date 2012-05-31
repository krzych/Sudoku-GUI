// ImageViewer.cpp : implementation file
//

#include "stdafx.h"
#include "Sudoku.h"
#include "ImageViewer.h"


// ImageViewer

IMPLEMENT_DYNAMIC(ImageViewer, CButton)

ImageViewer::ImageViewer()
{

}

ImageViewer::~ImageViewer()
{
}


BEGIN_MESSAGE_MAP(ImageViewer, CButton)
END_MESSAGE_MAP()



// ImageViewer message handlers


#include <opencv2\highgui\highgui_c.h>

void ImageViewer::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	static const int iloscd=2; 
	dc.Attach(lpDrawItemStruct->hDC);
	CRect rc=lpDrawItemStruct->rcItem;
	BITMAPINFO* bmi; 
	BITMAPINFOHEADER* bmih;
	RGBQUAD* palette;
	unsigned int buffer[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256]; 
//	CRect rc;
//	GetClientRect(&rc);
	IplImage image = m_img;
	if(image.imageData)
	{
		bmi = (BITMAPINFO*)buffer;
		bmih = &(bmi->bmiHeader);
		memset( bmih, 0, sizeof(*bmih));
		bmih->biSize= sizeof(BITMAPINFOHEADER); 
		bmih->biWidth = image.width;
		bmih->biHeight = -image.height;
		bmih->biPlanes = 1; 
		bmih->biCompression = BI_RGB;
		bmih->biBitCount = 8 *image.nChannels;
		palette = bmi->bmiColors;
		if(image.nChannels == 1)
		{
			for( int i = 0; i < 256; i++ )
			{
				palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
				palette[i].rgbReserved = 0;
			}
		}
		int startX,startY,a,b;
		CSize sizeTotal;
		dc.SetStretchBltMode(HALFTONE);
		CvSize s=cvGetSize(&image);
		if(rc.Height()*s.width<rc.Width()*s.height)
		{
			a=rc.Height();
			b=s.width*a/s.height;
		}
		else
		{
			b=rc.Width();
			a=s.height*b/s.width;
		}
		if(rc.Width()>b) startX=(rc.Width()-b)/2;
		else startX=0;
		if(rc.Height()>a) startY=(rc.Height()-a)/2;
		else startY=0;

		int res = StretchDIBits(
			dc.GetSafeHdc(), //dc
			startX, //x dest
			startY, //y dest
			b, //x dest dims
			a, //y dest dims
			0, // src x
			0, // src y
			image.width, // src dims
			image.height, // src dims
			image.imageData, // array of DIB bits
			(BITMAPINFO*)bmi, // bitmap information
			DIB_RGB_COLORS, // RGB or palette indexes
			SRCCOPY); // raster operation code
		if(startX!=0)
		{
			dc.FillSolidRect(rc.left,rc.top,startX,rc.Height(),RGB(194,217,247)); //RGB(255,255,255));
			dc.FillSolidRect(rc.left+startX+b,rc.top,rc.Width()-b-startX,rc.Height(),RGB(194,217,247)); //RGB(255,255,255));
		}
		if(startY!=0)
		{
			dc.FillSolidRect(rc.left,rc.top,rc.Width(),startY,RGB(194,217,247)); //RGB(255,255,255));
			dc.FillSolidRect(rc.left,rc.top+startY+a,rc.Width(),rc.Height()-a-startY,RGB(194,217,247)); //RGB(255,255,255));
		}
	}
	else
	{
		dc.FillSolidRect(rc,RGB(194,217,247));
	}
	dc.DeleteDC();
}
