
// SudokuDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sudoku.h"
#include "SudokuDlg.h"
#include "afxdialogex.h"
#include "core\NormalizedCentralMomentsRecognizer.h"
#include <vector>
#include "core\MathUtilities.h"
#include "SudokuExtracotrDlg.h"
#include "DigitExtractorDlg.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSudokuDlg dialog




CSudokuDlg::CSudokuDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSudokuDlg::IDD, pParent)
	, m_strPath(_T("D:/Dropbox/SudokuApp/SudokuApp/foto1.JPG"))
	, m_strStatus(_T(""))
	, m_SEDlg(NULL)
  , server(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSudokuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DISP_INPUT, m_DispInput);
	DDX_Control(pDX, IDC_DISP_OUTPUT, m_DispOutput);
	DDX_Text(pDX, IDC_EDIT_PATH_TO_IMG, m_strPath);
	DDX_Control(pDX, IDC_BTN_START, m_btnStart);
	DDX_Text(pDX, IDC_TXT_STATUS, m_strStatus);
	DDX_Control(pDX, IDC_BTN_SUDOKU_EXTRACTOR, m_btnSudokuEtractor);
	DDX_Control(pDX, IDC_BTN_DIGIT_EXTRACTOR2, m_btnDigitExtractor);
	DDX_Control(pDX, IDC_BTN_ACQUIRE, m_btnAcquireImage);
}

BEGIN_MESSAGE_MAP(CSudokuDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_ACQUIRE, &CSudokuDlg::OnBnClickedBtnAcquire)
	ON_BN_CLICKED(IDC_BTN_START, &CSudokuDlg::OnBnClickedBtnStart)

	ON_BN_CLICKED(IDC_BTN_SUDOKU_EXTRACTOR, &CSudokuDlg::OnBnClickedBtnSudokuExtractor)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_DIGIT_EXTRACTOR2, &CSudokuDlg::OnBnClickedBtnDigitExtractor2)
	ON_BN_CLICKED(IDC_BUTTON1, &CSudokuDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSudokuDlg message handlers

BOOL CSudokuDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	DisableButtons();
	m_strStatus = CString(_T("Ready."));
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSudokuDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSudokuDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSudokuDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSudokuDlg::OnBnClickedBtnAcquire()
{
	//UpdateData(TRUE);
//	TRACE(_T("%s\n"), m_strPath);
	m_InputImg = m_ImageAcquirer.AcquireImage(CStringA(m_strPath));
	m_DispOutput.SetImg(Mat());
	if(m_InputImg.data == NULL) {
		m_strStatus = CString(_T("Wrong path."));
		DisableButtons();
		m_DispInput.SetImg(Mat());
	} else {
		m_DispInput.SetImg(m_InputImg);
		m_strStatus = CString(_T("Image loaded."));
		//UpdateData(FALSE);
		EnableButtons();
		m_ImageProcessing.LoadImageW(m_InputImg);
	}
	//UpdateData(FALSE);
	
}

void CSudokuDlg::OnBnClickedBtnStart()
{
	AfxGetApp()->BeginWaitCursor();
	m_DispOutput.SetImg(Mat());
	m_pDigitRecognizer = new NormalizedCentralMomentsRecognizer();
	m_pDigitRecognizer->SetMode(DEFAULT_MODE);

	m_strStatus = CString(_T("Extracting cells."));
	UpdateData(FALSE);
	vector<SudokuCellImg> extractd_cell_imgs = m_ImageProcessing.Run(*this, m_strStatus);
	m_pDigitRecognizer->LoadInput(extractd_cell_imgs);

	m_strStatus = CString(_T("Recognizing digits."));
	UpdateData(FALSE);
	vector<SudokuCell> v = m_pDigitRecognizer->Run();

	m_SudokuSolver.Init();
	while(m_SudokuSolver.LoadInput(v) == false) {
		m_strStatus = CString(_T("Recognition failed. Switched to learing mode."));
		UpdateData(FALSE);
		m_pDigitRecognizer->SetMode(TRAINING_MODE);
		v = m_pDigitRecognizer->Run();
	}
	m_strStatus = CString(_T("Solving Sudoku"));
	UpdateData(FALSE);
	solution = m_SudokuSolver.Run();

  sendSolution(solution);

	m_SolutionViewer.LoadImageW(m_ImageProcessing.GetSudoku());
	v.clear();
	m_strStatus = CString(_T("Displaying solved SUDOKU"));
	UpdateData(FALSE);
	
	vec.clear();
	counter = 0;
	SetTimer(101, 200, NULL);


	delete m_pDigitRecognizer;
	
}

void CSudokuDlg::sendSolution(std::vector<SudokuCell> &solution)
{
  std::string lineToSend;
  unsigned int index = 0;
  for (int row = 0; row < 9; ++row) {
    for (int col = 0; col < 9; ++col) {
      SudokuCell cell = solution[index];
      if (cell.row == row && cell.col == col) {
        char buf[2];
        sprintf(buf, "%d", cell.val);
        lineToSend += buf;
        index = ++index%solution.size();
      }
      else {
        lineToSend += "0";
      }
    }
    // send
    lineToSend.clear();
  }
}

void CSudokuDlg::DisableButtons(void)
{
	m_btnStart.EnableWindow(FALSE);
	m_btnDigitExtractor.EnableWindow(FALSE);
	m_btnSudokuEtractor.EnableWindow(FALSE);
}

void CSudokuDlg::EnableButtons(void)
{
	m_btnStart.EnableWindow(TRUE);
	m_btnDigitExtractor.EnableWindow(TRUE);
	m_btnSudokuEtractor.EnableWindow(TRUE);
}

void CSudokuDlg::OnBnClickedBtnSudokuExtractor()
{
	m_ImageProcessing.LoadImageSE();
	CSudokuExtracotrDlg dlg(this, &m_ImageProcessing.sudoku_extractor);
	dlg.DoModal();
}


void CSudokuDlg::OnTimer(UINT_PTR nIDEvent)
{
	int num = MathUtilities::GetRandomNumber(0, solution.size() - 1);
		
	vec.push_back(solution[num]);
	solution.erase(solution.begin()+num);

	m_SolutionViewer.LoadSolution(vec);
	m_DispOutput.SetImg(m_SolutionViewer.View());

	if(solution.empty()) {
		KillTimer(101);
		m_strStatus = CString(_T("Ready."));
		UpdateData(FALSE);
		AfxGetApp()->EndWaitCursor();
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CSudokuDlg::OnBnClickedBtnDigitExtractor2()
{
	m_ImageProcessing.LoadImageDE();
	CDigitExtractorDlg dlg(this, &m_ImageProcessing.digit_extractor);
	dlg.DoModal();
}


void CSudokuDlg::OnBnClickedButton1()
{
	//CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T("picture files(*.jpg)|*.jpg||"), this);
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T("picture files (*.bmp,*.jpg,*.png)|*.bmp;*.jpg;*.png||"), this);
	if(dlg.DoModal() == IDOK) {
		m_strPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}