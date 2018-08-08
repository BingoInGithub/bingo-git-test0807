
// MFCApplication1Dlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include <afxwin.h>
#include "MatImageConversion.h"
#include <string>
//#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//全局变量
char str[] = "";//用于临时保存图像数据
static Point pre_pt(-1, -1);//初始坐标  
static Point cur_pt(-1, -1);//实时坐标 

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
//CImage image;
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


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_startRect = false;
	m_startCorner = false;
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication1Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTONChoosefile, &CMFCApplication1Dlg::OnBnClickedButtonChoosefile)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTONSave, &CMFCApplication1Dlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTONGenerate, &CMFCApplication1Dlg::OnBnClickedButtongenerate)
	ON_COMMAND(ID_mu_choosefile, &CMFCApplication1Dlg::OnMuChoosefile)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
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
	//CMenu menu;
	menu.LoadMenu(IDR_menu);
	SetMenu(&menu);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCApplication1Dlg::OnPaint()
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
		CDialogEx::OnPaint();
		
		CWnd* pWin = GetDlgItem(IDC_STATICPicture);//获得控件句柄 
		CDC *pDc = pWin->GetDC();

		CRect rect;
		pWin->GetClientRect(rect);//获得控件区域
		int rw = rect.Width();
		int rh = rect.Height();
		if (!image.IsNull())
		{
			int h = image.GetHeight();
			int w = image.GetWidth();
			int origin_x = 0;
			int origin_y = 0;
			
			//固定h,w相应缩放
			w = w * rh / h;
			h = rh;
			if (w > rw)
			{
				//固定w,h相应缩放
				h = h * rw / w;
				w = rw;
				origin_y += (rh - h) / 2;
			}
			else
				origin_x += (rw - w) / 2;
			image.Draw(pDc->GetSafeHdc(), origin_x, origin_y,w,h);
		}

		this->ReleaseDC(pDc);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

//点击“选择文件”按钮，选择模板图片
void CMFCApplication1Dlg::OnBnClickedButtonChoosefile()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePathName;

	//CFileDialog filedlg(true); //显示打开对话框
	CFileDialog filedlg(TRUE, NULL, NULL, 0, _T("All Files (*.*)|*.*|BMP (*.bmp)|*.bmp|DIB (*.dib)|*.dib|EMF (*.emf)|*.emf|GIF (*.gif)|*.gif|ICO (*.ico)|*.ico|JPG (*.jpg)|*.jpg|WMF (*.wmf)|*.wmf||"), NULL);
	filedlg.m_ofn.lpstrTitle = _T("打开");
	
	//filedlg.m_ofn.lpstrFilter = _T("Text Files(*.txt)/0*.txt/0All Files(*.*)/0*.*/0/0");

	//if (IDOK == filedlg.DoModal())

	//{

	//	//      strFileName = fileDlg.GetPathName();

	//	FilePathName = filedlg.GetPathName();

	//	//取出文件路径,将文件的路径放入FilePath

	//	UpdateData(FALSE);

	//	//UpdateData的参数标识数据更新的方向。FALSE 表示控件初始化，也就是从成员变量里面读取数据显示在控件上，

	//	//TRUE表示从控件读取数据到对应的成员变量



	//}

	//SetDlgItemText(IDC_EDIT1, FilePathName);
	
	//4.给按钮的函数里写上:(注意,这个IDC_SHOWPIC)是你那个显示控件的ID号.
	/*CRect rect;
	GetDlgItem(IDC_STATICPicture)->GetWindowRect(&rect);
	ScreenToClient(&rect);*/
	
	CFile f;
	//CString  FilePathName;
	CFileException e;
	//CFileDialog dlg(TRUE, NULL, NULL, 0, _T("All Files (*.*)|*.*|BMP (*.bmp)|*.bmp|DIB (*.dib)|*.dib|EMF (*.emf)|*.emf|GIF (*.gif)|*.gif|ICO (*.ico)|*.ico|JPG (*.jpg)|*.jpg|WMF (*.wmf)|*.wmf||"), NULL);
	
	if (filedlg.DoModal() == IDOK)
	{
		
		if (!image.IsNull())
		{
			image.Destroy();
		}
		image.Load(filedlg.GetPathName());
		MatImageConversion mic;
		mic.CImageToMat(image, org);
		//namedWindow("wind1", WINDOW_NORMAL);
		//resizeWindow("wind1",)
		//imshow("ddd", org);
		//Invalidate函数是使当前窗口无效并重绘窗口，重绘时调用WM_PAINT消息，把此处做的改变显示到窗口
		Invalidate();
	}
}
void CMFCApplication1Dlg::OnMuChoosefile()
{
	// TODO: 在此添加命令处理程序代码
	CString FilePathName;

	//CFileDialog filedlg(true); //显示打开对话框
	CFileDialog filedlg(TRUE, NULL, NULL, 0, _T("All Files (*.*)|*.*|BMP (*.bmp)|*.bmp|DIB (*.dib)|*.dib|EMF (*.emf)|*.emf|GIF (*.gif)|*.gif|ICO (*.ico)|*.ico|JPG (*.jpg)|*.jpg|WMF (*.wmf)|*.wmf||"), NULL);
	filedlg.m_ofn.lpstrTitle = _T("打开");
	CFile f;
	CFileException e;
	
	if (filedlg.DoModal() == IDOK)
	{

		if (!image.IsNull())
		{
			image.Destroy();
		}
		image.Load(filedlg.GetPathName());
		MatImageConversion mic;
		mic.CImageToMat(image, org);
		Invalidate();
	}
}

void CMFCApplication1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//AfxMessageBox(_T("左键按下"));
	if (m_startCorner == true)
	{
		start_pt = point;
		end_pt = point;
		m_startRect = true;

		pre_pt.x = point.x;
		pre_pt.y = point.y;

		//以下为实验部分，第二个DrawRect的作用是取反，因此执行两次相当于不执行
		/*CPoint pta, ptb,ptc,ptd;
		pta = point;
		ptb.x = point.x + 20;
		ptb.y = point.y + 20;
		ptc.x = point.x - 20;
		ptc.y = point.y - 20;
		ptd.x = ptb.x + 20;
		ptd.y = ptb.y + 20;
		DrawRect(pta, ptb);
		DrawRect(pta, ptb);*/
	}
	CDialogEx::OnLButtonDown(nFlags, point);
	
}


void CMFCApplication1Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_startRect == true && m_startCorner == true)
	{
		//pt = point;
		//CClientDC dc(this);
		//COLORREF col = RGB(255, 0, 0);
		//CPen pen(PS_SOLID, 2, col);
		//dc.SelectObject(&pen);
		////dc.SetROP2(R2_NOT);
		//(CBrush*)dc.SelectStockObject(NULL_BRUSH);
		//dc.Rectangle(CRect(start_pt, pt));
		//Invalidate();
		//UpdateWindow();
		//cur_pt.x = point.x;
		//cur_pt.y = point.y;
		//rectangle(tmp, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 4, 8, 0);//在临时图像上实时显示鼠标拖动时形成的矩形  

		//第一个DrawRect将上一次画的框擦除，第二个DrawRect画本次的框
		DrawRect(start_pt, end_pt);
		DrawRect(start_pt, point);
		end_pt = point;
		
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CMFCApplication1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_startCorner == true)
	{
		m_startRect = false;

		//当左键抬起，将矩形框的对角点记录入临时字符串中
		str += (to_string(start_pt.x)+" ");
		str += (to_string(start_pt.y) + " ");
		str += (to_string(end_pt.x) + " ");
		str += (to_string(end_pt.y) + " ");
	}
		
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMFCApplication1Dlg::DrawRect(CPoint ptUL, CPoint ptLR)
{
	CWnd* pWin = GetDlgItem(IDC_STATICPicture);//获得控件句柄
	CDC* pDC = pWin->GetDC();//得到该控件的画布，在画布上绘制

	CRect rect;
	pWin->GetClientRect(rect);//获得控件区域
	CRgn rgn;
	int h = rect.Height();
	int w = rect.Width();
	rgn.CreateRectRgn(0,0,w,h);
	pDC->SelectClipRgn(&rgn);//限制在rgn区域内画图
	
	
	// 设置透明画刷
	CBrush brush;
	//brush.CreateSolidBrush(PS_SOLID, 2, RGB(0, 255, 0));
	CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush *pOldBrush = pDC->SelectObject(pBrush);
	
	if (1)
	{
		pDC->SetROP2(R2_NOT);
		pDC->Rectangle(rect);
		pDC->Rectangle(ptUL.x, ptUL.y, ptLR.x, ptLR.y);
	}
	else
	{
		CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
		CPen *pOldPen = pDC->SelectObject(&pen);
		pDC->Rectangle(ptUL.x, ptUL.y, ptLR.x, ptLR.y);
	}

	ReleaseDC(pDC);

}
//建立一个新类，对选择的模板图片进行操作，提取图片信息

//选择保存路径，将路径存到FilePathName中
void CMFCApplication1Dlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	//CFileDialog filedlg(true); //显示打开对话框
	//AfxMessageBox(_T("保存"));
	/*for (int i = 0; i < 10; i++)
	{
		int a;
	}
	int a;
	int a;*/
	CString FilePathName;
	CFileDialog filedlg(FALSE, NULL, NULL, 0, _T("TXT (*.txt)|*.txt||"), NULL);
	filedlg.m_ofn.lpstrTitle = _T("打开");
	if (filedlg.DoModal() == IDOK)
	{
		FilePathName = filedlg.GetPathName();
		//UpdateData(FALSE);
		//AfxMessageBox(FilePathName);
		/*MatImageConversion mic;
		mic.CImageToMat(image, org);*/
		//将临时字符串str存入TXT
		fstream fout(FilePathName);
		int str_size = str.size();
		for (int i = 0; i < str_size; i++)
		{
			fout << str[i];
			if (i == str_size - 1)
				AfxMessageBox(_T("已保存"));
		}
		fout.close();
		//imshow("ddd", org);
		//Invalidate函数是使当前窗口无效并重绘窗口，重绘时调用WM_PAINT消息，把此处做的改变显示到窗口
		Invalidate();
	}
}
//得到上述路径，打开这个文件，进行输入
//整个程序的逻辑是：选择一幅图片，建立一个临时TXT文件，将当前图片特征点保存到文件中，使用鼠标画
//框，将角点保存到文件中，点击保存，将临时文件拷贝到最终文件中，清空临时文件，进入下一个循环

//点击“生成特征点”按钮，
void CMFCApplication1Dlg::OnBnClickedButtongenerate()
{
	// TODO: 在此添加控件通知处理程序代码
	//生成一个string字符串，存到字符串中，最后再存到TXT中
	//ofstream fout("E:\\VSFile\\tempTXT.txt");
	//将fout传入imgdata是否可行？无法传入

	//打开绘制角点的开关
	m_startCorner = true;
	//生成特征点与特征描述符
	imgdata.get_ImgData(org);
	str += "\n ";
	str = imgdata.get_str();
	AfxMessageBox(_T("已生成"));
	//下面一段代码可将str写入TXT中
	/*ofstream fout("E:\\VSFile\\tempTXT.txt");
	int str_size = str.size();
	for (int i = 0; i < str_size; i++)
	{
		fout << str[i];
		if (i == str_size - 1)
			AfxMessageBox(_T("已生成"));
	}
	fout.close();*/
	

}



