#pragma once
#include "afxcmn.h"
#include "imageviewer.h"
#include "core\DigitExtractor.h"


// CDigitExtractorDlg dialog

class CDigitExtractorDlg : public CDialog
{
	DECLARE_DYNAMIC(CDigitExtractorDlg)

public:
	CDigitExtractorDlg(CWnd* pParent = NULL);   // standard constructor
	CDigitExtractorDlg(CWnd* pParent, DigitExtractor* p_DigitExtractor);
	virtual ~CDigitExtractorDlg();

// Dialog Data
	enum { IDD = IDD_DIGIT_EXTRACTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strBlockSize;
	CString m_strC;
	CString m_strPercentage;
	CString m_strN;
	CString m_strKernelSizeMorph;
	CSliderCtrl m_SliderBlockSize;
	CSliderCtrl m_SliderC;
	CSliderCtrl m_SliderPercentage;
	CSliderCtrl m_SliderN;
	CSliderCtrl m_SliderKernelSizeMorph;
	ImageViewer m_DispCell;
	ImageViewer m_DispSudoku;
	virtual BOOL OnInitDialog();

private:
	DigitExtractor* m_pDigitExtractor;
	int row;
	int col;
public:
	afx_msg void OnBnClickedBtnDefault();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnProcess();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton2();
};
