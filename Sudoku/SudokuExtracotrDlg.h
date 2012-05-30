#pragma once
#include "afxcmn.h"
#include "imageviewer.h"
#include "afxwin.h"
#include "core\SudokuExtractor.h"

// CSudokuExtracotrDlg dialog

class CSudokuExtracotrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSudokuExtracotrDlg)

public:
	CSudokuExtracotrDlg(CWnd* pParent = NULL);   // standard constructor
	CSudokuExtracotrDlg(CWnd* pParent, SudokuExtractor* sudokuExtractor);  
	virtual ~CSudokuExtracotrDlg();

// Dialog Data
	enum { IDD = IDD_SUDOKU_EXTRACTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	SudokuExtractor* m_SudokuExtractor;
	CSliderCtrl m_SliderKernelSizeGauss;
	CSliderCtrl m_SliderBlockSize;
	CSliderCtrl m_SliderC;
	CSliderCtrl m_SliderContourAreaThresh;
	CSliderCtrl m_SliderKernelSizeMorph;
	ImageViewer m_Display;
	CString m_strKernelSizeGauss;
	CString m_strBlockSize;
	CString m_strC;
	CString m_strKernelSizeMorph;
	CString m_strContourAreaThresh;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnDone();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnDefault();
	afx_msg void OnBnClickedBtnOk();
	CProgressCtrl m_ProgressBar;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
