// SudokuExtracotrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sudoku.h"
#include "SudokuExtracotrDlg.h"
#include "afxdialogex.h"


// CSudokuExtracotrDlg dialog

IMPLEMENT_DYNAMIC(CSudokuExtracotrDlg, CDialogEx)

CSudokuExtracotrDlg::CSudokuExtracotrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSudokuExtracotrDlg::IDD, pParent)
	, m_strKernelSizeGauss(_T(""))
	, m_strBlockSize(_T(""))
	, m_strC(_T(""))
	, m_strKernelSizeMorph(_T(""))
	, m_strContourAreaThresh(_T(""))
	, m_SudokuExtractor(NULL)
{

}

CSudokuExtracotrDlg::CSudokuExtracotrDlg(CWnd* pParent, SudokuExtractor* sudokuExtractor)
	: CDialogEx(CSudokuExtracotrDlg::IDD, pParent)
	, m_strKernelSizeGauss(_T(""))
	, m_strBlockSize(_T(""))
	, m_strC(_T(""))
	, m_strKernelSizeMorph(_T(""))
	, m_strContourAreaThresh(_T(""))
	, m_SudokuExtractor(sudokuExtractor)
{

}

CSudokuExtracotrDlg::~CSudokuExtracotrDlg()
{
	m_SudokuExtractor->ClearAdjustFlags();
}

void CSudokuExtracotrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_SliderKernelSizeGauss);
	DDX_Control(pDX, IDC_SLIDER2, m_SliderBlockSize);
	DDX_Control(pDX, IDC_SLIDER3, m_SliderC);
	DDX_Control(pDX, IDC_SLIDER4, m_SliderContourAreaThresh);
	DDX_Control(pDX, IDC_SLIDER5, m_SliderKernelSizeMorph);
	DDX_Control(pDX, IDC_BUTTON1, m_Display);
	DDX_Text(pDX, IDC_IND_KERNEL_SIZE_GAUSS, m_strKernelSizeGauss);
	DDX_Text(pDX, IDC_IND_BLOCK_SIZE, m_strBlockSize);
	DDX_Text(pDX, IDC_IND_C, m_strC);
	DDX_Text(pDX, IDC_IND_KERNEL_SIZE_MORPH, m_strKernelSizeMorph);
	DDX_Text(pDX, IDC_IND_CONTOUR_AREA_THRESH, m_strContourAreaThresh);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
}


BEGIN_MESSAGE_MAP(CSudokuExtracotrDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_DONE, &CSudokuExtracotrDlg::OnBnClickedBtnDone)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_DEFAULT, &CSudokuExtracotrDlg::OnBnClickedBtnDefault)
	ON_BN_CLICKED(IDC_BTN_OK, &CSudokuExtracotrDlg::OnBnClickedBtnOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSudokuExtracotrDlg message handlers


BOOL CSudokuExtracotrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	AfxGetApp()->BeginWaitCursor();
	// TODO:  Add extra initialization here
	int pos;
	m_SliderKernelSizeGauss.SetRange(1, 10);
	pos = m_SudokuExtractor->GetKernelSizeGauss();
	m_SliderKernelSizeGauss.SetPos((pos-1)/2);
	m_strKernelSizeGauss.Format(_T("%d"), pos);

	m_SliderBlockSize.SetRange(1, 10);
	pos = m_SudokuExtractor->GetBlockSize();
	m_SliderBlockSize.SetPos((pos-1)/2);
	m_strBlockSize.Format(_T("%d"), pos);

	m_SliderC.SetRange(1, 10);
	pos = (int)m_SudokuExtractor->GetC();
	m_SliderC.SetPos(pos);
	m_strC.Format(_T("%d"), pos);

	m_SliderContourAreaThresh.SetRange(1, 2000);
	pos = m_SudokuExtractor->GetContourAreaThresh();
	m_SliderContourAreaThresh.SetPos(pos);
	m_strContourAreaThresh.Format(_T("%d"), pos);

	m_SliderKernelSizeMorph.SetRange(1, 10);
	pos = m_SudokuExtractor->GetKernelSizeMorph();
	m_SliderKernelSizeMorph.SetPos((pos-1)/2);
	m_strKernelSizeMorph.Format(_T("%d"), pos);

	m_ProgressBar.SetRange(0, 4);
	m_ProgressBar.SetPos(0);
	m_ProgressBar.SetStep(1);
	UpdateData(FALSE);

	m_ProgressBar.ShowWindow(SW_SHOW);
	m_SudokuExtractor->SetAdjustFlag();
	m_Display.SetImg(m_SudokuExtractor->Preprocess(&m_ProgressBar));
	AfxGetApp()->EndWaitCursor();
	SetTimer(111, 1000, NULL);
	
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSudokuExtracotrDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(nSBCode == 8) {
		int pos;
		if(pScrollBar->GetSafeHwnd() == m_SliderBlockSize.GetSafeHwnd()) {
			pos = m_SliderBlockSize.GetPos()*2+1;
			m_SudokuExtractor->SetBlockSize(pos);
			m_strBlockSize.Format(_T("%d"), pos);
		}
		if(pScrollBar->GetSafeHwnd() == m_SliderC.GetSafeHwnd()) {
			pos = m_SliderC.GetPos();
			m_SudokuExtractor->SetC(pos);
			m_strC.Format(_T("%d"), pos);
		}
		if(pScrollBar->GetSafeHwnd() == m_SliderContourAreaThresh.GetSafeHwnd()) {
			pos = m_SliderContourAreaThresh.GetPos();
			m_SudokuExtractor->SetContourAreaThresh(pos);
			m_strContourAreaThresh.Format(_T("%d"), pos);
		}
		if(pScrollBar->GetSafeHwnd() == m_SliderKernelSizeGauss.GetSafeHwnd()) {
			pos = m_SliderKernelSizeGauss.GetPos()*2+1; //must be uneven
			m_SudokuExtractor->SetKernelSizeGauss(pos); 
			m_strKernelSizeGauss.Format(_T("%d"), pos);
		}
		if(pScrollBar->GetSafeHwnd() == m_SliderKernelSizeMorph.GetSafeHwnd()) {
			pos = m_SliderKernelSizeMorph.GetPos()*2+1; //must be uneven
			m_SudokuExtractor->SetKernelSizeMorph(pos);
			m_strKernelSizeMorph.Format(_T("%d"), pos);
		}

		UpdateData(FALSE);
	}
	

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CSudokuExtracotrDlg::OnBnClickedBtnDone()
{
	// TODO: Add your control notification handler code here
	AfxGetApp()->BeginWaitCursor();
	m_ProgressBar.SetPos(0);
	m_ProgressBar.ShowWindow(SW_SHOW);	
	UpdateData(FALSE);
	m_Display.SetImg(m_SudokuExtractor->Preprocess(&m_ProgressBar));
	SetTimer(111, 1000, NULL);
	AfxGetApp()->EndWaitCursor();
}

//extern CSudokuExtracotrDlg* m_SEDlg;
void CSudokuExtracotrDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnClose();
}


void CSudokuExtracotrDlg::OnBnClickedBtnDefault()
{
	// TODO: Add your control notification handler code here
	m_SliderBlockSize.SetPos(2);
	m_strBlockSize = _T("5");
	m_SudokuExtractor->SetBlockSize(5);

	m_SliderC.SetPos(2);
	m_strC = _T("2");
	m_SudokuExtractor->SetC(2);

	m_SliderContourAreaThresh.SetPos(1000);
	m_strContourAreaThresh = _T("1000");
	m_SudokuExtractor->SetContourAreaThresh(1000);

	m_SliderKernelSizeGauss.SetPos(5);
	m_strKernelSizeGauss = _T("11");
	m_SudokuExtractor->SetKernelSizeGauss(11);

	m_SliderKernelSizeMorph.SetPos(1);
	m_strKernelSizeMorph = _T("3");
	m_SudokuExtractor->SetKernelSizeMorph(3);

	UpdateData(FALSE);
}


void CSudokuExtracotrDlg::OnBnClickedBtnOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnClose();
	EndDialog(0);
}


void CSudokuExtracotrDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	m_ProgressBar.ShowWindow(SW_HIDE);
	KillTimer(111);
	CDialogEx::OnTimer(nIDEvent);
}
