
// aDlg.cpp: 实现文件
//

#include "pch.h"
#include "MatCImage.h"
#include "framework.h"
#include "a.h"
#include "aDlg.h"
#include "afxdialogex.h"
#include <opencv2/opencv.hpp>;
#include <opencv2\imgproc\types_c.h>;
using namespace cv;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
CImage image;
CImage img1;
Mat img,im2,im3;
Mat scaledImage;
RECT pic_rect;
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CaDlg 对话框



CaDlg::CaDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_A_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CaDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CaDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CaDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CaDlg 消息处理程序

BOOL CaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CaDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		
		if (!img1.IsNull())
		{
			//显示CImage图片
			
			CWnd* pWnd = GetDlgItem(IDC_STATIC);//获得pictrue控件窗口的句柄   
			CDC* pDC = pWnd->GetDC();//获得pictrue控件的DC  
			if (!img1.IsNull())
			{
				img1.Draw(pDC->m_hDC, pic_rect); //将图片画到Picture控件表示的矩形区域    
			}

			ReleaseDC(pDC);//释放picture控件的DC  
		}

		//不注释掉下面这一句会出现残留等很多问题
		//CDialogEx::OnPaint();  
	}
	
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CaDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString strFilter = _T("所有文件(*.*)|*.*|");
	//CFileDialog dlg(TRUE, NULL, NULL, NULL, strFilter, this);
	//SetDlgItemText(IDC_ImagePath, _T(" "));
	CFileDialog dlg(true, _T("*.bmp"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.bmp ;*.jpg)|*.bmp;*.png;*.jpg |ALL Files (*.*) |*.*||"), NULL);

	if (!dlg.DoModal() == IDOK)
		return;
	CString strFileName = dlg.GetPathName();
	CFileStatus status;
	if (!CFile::GetStatus(strFileName, status))
	{
		MessageBox(strFileName + "不存在", L"信息提示", MB_OK);
		return;
	}

	image.Load(strFileName);

	//把CImage转化为Mat
	MatCImage MatCImage;
	MatCImage.CImageToMat(image, img);
	//获取图片控件的大小
	GetDlgItem(IDC_STATIC)->GetClientRect(&pic_rect);
	long width = pic_rect.right;
	long height = pic_rect.bottom;

	//改变图片大小适应picture控件
	cvtColor(img, im2, CV_BGR2GRAY);
	adaptiveThreshold(im2, im3, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 10);
	resize(im3, scaledImage, Size(width, height));

	//Mat转换为CImage
	if (!scaledImage.empty())
	{
		MatCImage.MatToCImage(scaledImage, img1);
	}
	
}


void CaDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}
