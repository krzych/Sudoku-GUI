// DigitExtractorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sudoku.h"
#include "DigitExtractorDlg.h"
#include "afxdialogex.h"


// CDigitExtractorDlg dialog

IMPLEMENT_DYNAMIC(CDigitExtractorDlg, CDialog)

CDigitExtractorDlg::CDigitExtractorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDigitExtractorDlg::IDD, pParent)
	, m_strBlockSize(_T(""))
	, m_strC(_T(""))
	, m_strPercentage(_T(""))
	, m_strN(_T(""))
	, m_strKernelSizeMorph(_T(""))
	, row(0)
	, col(0)
{

}

CDigitExtractorDlg::CDigitExtractorDlg(CWnd* pParent, DigitExtractor* digitExtractor)
	: CDialog(CDigitExtractorDlg::IDD, pParent)
	, m_strBlockSize(_T(""))
	, m_strC(_T(""))
	, m_strPercentage(_T(""))
	, m_strN(_T(""))
	, m_strKernelSizeMorph(_T(""))
	, m_pDigitExtractor(digitExtractor)
	, row(0)
	, col(0)
{
	m_pDigitExtractor->SetAdjustFlag();
}

CDigitExtractorDlg::~CDigitExtractorDlg()
{
	m_pDigitExtractor->ClearAdjustFlags();
	
}

void CDigitExtractorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IND_BLOCK_SIZE, m_strBlockSize);
	DDX_Text(pDX, IDC_IND_C, m_strC);
	DDX_Text(pDX, IDC_IND_PERCENTAGE, m_strPercentage);
	DDX_Text(pDX, IDC_IND_N, m_strN);
	DDX_Text(pDX, IDC_IND_THRESH, m_strKernelSizeMorph);
	DDX_Control(pDX, IDC_SLIDER1, m_SliderBlockSize);
	DDX_Control(pDX, IDC_SLIDER2, m_SliderC);
	DDX_Control(pDX, IDC_SLIDER3, m_SliderPercentage);
	DDX_Control(pDX, IDC_SLIDER4, m_SliderN);
	DDX_Control(pDX, IDC_SLIDER6, m_SliderKernelSizeMorph);
	DDX_Control(pDX, IDC_BUTTON1, m_DispCell);
	DDX_Control(pDX, IDC_BUTTON2, m_DispSudoku);
}


BEGIN_MESSAGE_MAP(CDigitExtractorDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_DEFAULT, &CDigitExtractorDlg::OnBnClickedBtnDefault)
	ON_BN_CLICKED(IDC_BTN_OK, &CDigitExtractorDlg::OnBnClickedBtnOk)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_PROCESS, &CDigitExtractorDlg::OnBnClickedBtnProcess)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON2, &CDigitExtractorDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDigitExtractorDlg message handlers


BOOL CDigitExtractorDlg::OnInitDialog()
{
	AfxGetApp()->BeginWaitCursor();

	CDialog::OnInitDialog();
	
	int pos;
	m_SliderBlockSize.SetRange(1, 10);
	pos = m_pDigitExtractor->GetBlockSize();
	m_SliderBlockSize.SetPos((pos-1)/2);
	m_strBlockSize.Format(_T("%d"), pos);

	m_SliderC.SetRange(1, 10);
	pos = (int)m_pDigitExtractor->GetC();
	m_SliderC.SetPos(pos);
	m_strC.Format(_T("%d"), pos);

	m_SliderN.SetRange(1, 5);
	pos = m_pDigitExtractor->GetN();
	m_SliderN.SetPos(pos);
	m_strN.Format(_T("%d"), pos);

	m_SliderPercentage.SetRange(0, 50);
	pos = m_pDigitExtractor->GetPercentage();
	m_SliderPercentage.SetPos(pos);
	m_strPercentage.Format(_T("%d"), pos);

	m_SliderKernelSizeMorph.SetRange(1, 10);
	pos = m_pDigitExtractor->GetKernelSizeMorph();
	m_SliderKernelSizeMorph.SetPos((pos-1)/2);
	m_strKernelSizeMorph.Format(_T("%d"), pos);
	UpdateData(FALSE);

	
	m_DispSudoku.SetImg(m_pDigitExtractor->GetInputImage());
	m_DispCell.SetImg(m_pDigitExtractor->GetProcessedCell(col, row));
	AfxGetApp()->EndWaitCursor();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDigitExtractorDlg::OnBnClickedBtnDefault()
{
	m_pDigitExtractor->SetBlockSize(7);
	m_SliderBlockSize.SetPos(3);
	m_strBlockSize = _T("7");

	m_pDigitExtractor->SetC(2);
	m_SliderC.SetPos(2);
	m_strC = _T("2");

	m_pDigitExtractor->SetKernelSizeMorph(3);
	m_SliderKernelSizeMorph.SetPos(1);
	m_strKernelSizeMorph = _T("3");

	m_pDigitExtractor->SetN(2);
	m_SliderN.SetPos(2);
	m_strN = _T("2");

	m_pDigitExtractor->SetPercentage(20);
	m_SliderPercentage.SetPos(20);
	m_strPercentage = _T("20");

	UpdateData(FALSE);
}


void CDigitExtractorDlg::OnBnClickedBtnOk()
{
	EndDialog(0);
}


void CDigitExtractorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(nSBCode == 8) {
		int pos = m_SliderBlockSize.GetPos()*2+1;
		m_pDigitExtractor->SetBlockSize(pos);
		m_strBlockSize.Format(_T("%d"), pos);

		pos = m_SliderC.GetPos();
		m_pDigitExtractor->SetC(pos);
		m_strC.Format(_T("%d"), pos);

		pos = m_SliderKernelSizeMorph.GetPos()*2+1;
		m_pDigitExtractor->SetKernelSizeMorph(pos);
		m_strKernelSizeMorph.Format(_T("%d"), pos);

		pos = m_SliderN.GetPos();
		m_pDigitExtractor->SetN(pos);
		m_strN.Format(_T("%d"), pos);

		pos = m_SliderPercentage.GetPos();
		m_pDigitExtractor->SetPercentage(pos);
		m_strPercentage.Format(_T("%d"), pos);

		UpdateData(FALSE);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDigitExtractorDlg::OnBnClickedBtnProcess()
{
	AfxGetApp()->BeginWaitCursor();
	m_DispCell.SetImg(m_pDigitExtractor->GetProcessedCell(col, row));
	AfxGetApp()->EndWaitCursor();
}

void CDigitExtractorDlg::OnBnClickedButton2()
{
	CPoint point;
	GetCursorPos(&point);
	m_DispSudoku.ScreenToClient(&point);
//	TRACE(_T("x = %d y = %d\n"), point.x, point.y);

	CRect rect; 
	m_DispSudoku.GetWindowRect(&rect);

	int w = (int)floor(rect.Width()/9.0);
	int h = (int)floor(rect.Height()/9.0);

	col = 0;
	row = 0;

	while(point.x > (col+1)*w) col++;
	while(point.y > (row+1)*h) row++;
//	TRACE(_T("col = %d row = %d\n"), col, row);

	OnBnClickedBtnProcess();
}
