
// SudokuDlg.h : header file
//

#pragma once
#include "imageviewer.h"
#include "core\ImageAcquirer.h"
#include "core\ImageProcessing.h"
#include "core\DigitRecognizerInterface.h"
#include "core\SudokuSolver.h"
#include "core\SolutionSender.h"
#include "core\SolutionViewer.h"
#include "afxwin.h"
#include "SudokuExtracotrDlg.h"
#include "ServerThread.h"
#include "resource.h"

using namespace std;

// CSudokuDlg dialog
class CSudokuDlg : public CDialogEx
{
// Construction
public:
	CSudokuDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
 enum { IDD = IDD_SUDOKU_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	ImageViewer m_DispInput;
	ImageViewer m_DispOutput;

private:
  void sendSolution(std::vector<SudokuCell> &solution);

	ImageAcquirer m_ImageAcquirer;
	ImageProcessing m_ImageProcessing;
	DigitRecognizerInterface* m_pDigitRecognizer;
	SudokuSolver m_SudokuSolver;
	SolutionViewer m_SolutionViewer;
	Mat m_InputImg;
	CSudokuExtracotrDlg* m_SEDlg;
	std::vector<SudokuCell> solution;
	std::vector<SudokuCell> vec;
	int counter;
  ServerThread server;

public:
	void DisableButtons(void);
	void EnableButtons(void);
	afx_msg void OnBnClickedBtnAcquire();
	CString m_strPath;
	CButton m_btnStart;
	afx_msg void OnBnClickedBtnStart();
	CString m_strStatus;
	CButton m_btnSudokuEtractor;
	CButton m_btnDigitExtractor;
	afx_msg void OnBnClickedBtnSudokuExtractor();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnDigitExtractor2();
	CButton m_btnAcquireImage;
	afx_msg void OnBnClickedButton1();
};
