
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
#include "CvvImage.h"
#include "ChooseTarget.h"
#include "BoxNum.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//全局变量
//char str[] = "";//用于临时保存图像数据
static Point pre_pt(-1, -1);//初始坐标  
static Point cur_pt(-1, -1);//实时坐标 
bool whileflag = false;


struct fileData {
	vector<CString> filePath;
	CMFCApplication1Dlg* tagthis;
}fdata;
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
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);

	m_startRect = false;
	m_startCorner = false;
	chooseflag = false;
	generateflag = false;
	file_num = 0;
	box_num = 0;

	str = "";
	targetNum = 0;

	FoldName = "";

	tracker = TrackerMIL::create();
	updateflag = true;

	rotationflag = 0;
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_BUTTONSubmit, m_submitctrl);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_mu_choosefile, &CMFCApplication1Dlg::OnMuChoosefile)
	ON_COMMAND(ID_mu_generate, &CMFCApplication1Dlg::OnMuGenerate)
	ON_COMMAND(ID_mu_save, &CMFCApplication1Dlg::OnMuSave)
	ON_COMMAND(ID_mu_choosetarget, &CMFCApplication1Dlg::OnMuMatch)
	ON_COMMAND(ID_mu_video, &CMFCApplication1Dlg::OnMuVideo)
	ON_COMMAND(IDC_BUTTONSubmit, &CMFCApplication1Dlg::OnButtonSubmit)

	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnClickedButton2)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_mu_templates, &CMFCApplication1Dlg::OnMuTemplates)
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
	ShowSubmitCtrl(false);
	ShowRotationCtrl(false);
	//CMenu menu;
	menu.LoadMenu(IDR_menu);
	SetMenu(&menu);

	/*HBITMAP hbitmap;
	hbitmap = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON3));
	((CButton *)GetDlgItem(IDC_BUTTON1))->SetBitmap(hbitmap);*/

	//::SetWindowText(AfxGetMainWnd()->GetSafeHwnd(), _T("要设置的应用工程名字"));

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

//如果向对话框添加最小化按钮，则需要下面的代码来绘制该图标。 
//对于使用文档/视图模型的 MFC 应用程序，这将由框架自动完成。
//BOOL CMFCApplication1Dlg::OnEraseBkgnd(CDC* pDC)
//{
//	//return TRUE;
//	return CDialogEx::OnEraseBkgnd(pDC);
//}
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
		//AfxMessageBox(_T("here"));
	}
	else
	{
		CDialogEx::OnPaint();
		ShowRotationCtrl(false);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCApplication1Dlg::DrawEllipse(CPoint point, int r)
{
	CClientDC dc(this);
	CBrush brush, *oldbrush;
	brush.CreateSolidBrush(RGB(0xD1, 0xFB, 0xED));
	oldbrush = dc.SelectObject(&brush);
	dc.Ellipse(point.x - r, point.y - r, point.x + r, point.y + r);
	dc.SelectObject(oldbrush);
}
void CMFCApplication1Dlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//ShowSubmitCtrl(true);
	CDialogEx::OnRButtonDown(nFlags, point);
}
void CMFCApplication1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//AfxMessageBox(_T("左键按下"));
	//if (m_startCorner == true)AfxMessageBox(_T("true"));
	//else AfxMessageBox(_T("false"));
	//左键单击画点
	//DrawEllipse(point, 2);
	//showbnicon();
	//CEdit *edit = (CEdit*)GetDlgItem(IDC_BUTTONSubmit);
	//KillTimer(WAIT_TIMER);
	//SendMessage(WM_CLOSE, 0, 0);
	//GetDlgItem(IDC_BUTTONSubmit)->ShowWindow(SW_HIDE);
	//ShowSubmitCtrl(0);
	/*CPoint a(0, 0);
	CPoint b(100, 100);
	ControlToImg(a, org);
	ControlToImg(b, org);
	drawrect(a, b);*/
	if (m_startCorner == true)
	{
		//AfxMessageBox(_T("开始绘制矩形"));
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
	//CString strx;
	//org = imread("E:\\Picture\\scene0830\\template-0.jpg");
	//ControlToImg(point, org);
	//strx.Format(_T("%d,%d"), point.x,point.y);
	////stry.Format(_T("%d"), point.y);
	//CWnd* pWin = GetDlgItem(IDC_STATICPicture);//获得控件句柄
	//CDC* pDC = pWin->GetDC();//得到该控件的画布，在画布上绘制
	//ClearControl();
	//pDC->TextOut(point.x, point.y-15, strx);
	if (m_startRect == true && m_startCorner == true)
	{
		//第一个DrawRect将上一次画的框擦除，第二个DrawRect画本次的框
		DrawRect(start_pt, end_pt);
		DrawRect(start_pt, point);
		end_pt = point;

	}
	CDialogEx::OnMouseMove(nFlags, point);
}
void CMFCApplication1Dlg::drawrect(CPoint a, CPoint b)
{
	CWnd* pWin = GetDlgItem(IDC_STATICPicture);//获得控件句柄
	CDC* pDC = pWin->GetDC();//得到该控件的画布，在画布上绘制
	CPen pen;
	CPoint c(10, 100);
	pen.CreatePen(BS_SOLID, 1, RGB(255, 0, 0));
	pDC->SelectObject(&pen);
	pDC->MoveTo(a);
	pDC->LineTo(b);
	pDC->LineTo(c);
}
void CMFCApplication1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (start_pt == end_pt)
		m_startRect = false;
	if ((m_startCorner == true) && (start_pt != end_pt))
	{
		m_startRect = false;

		//交互输入方框序号和螺栓编号
		BoxNum boxnumdlg;
		boxnumdlg.DoModal();
		//方框序号
		int lbu_boxnum = 0;
		lbu_boxnum = boxnumdlg.getbnboxnum();
		if (lbu_boxnum > 0 && lbu_boxnum < 100)
		{
			//螺栓编号
			string lbu_boltnum = boxnumdlg.getbnboltnum();

			box_num = box_num > lbu_boxnum ? box_num : lbu_boxnum;


			//显示方框序号
			CString box_str;
			box_str.Format(_T(" [%d] "), lbu_boxnum);
			CWnd* pWin = GetDlgItem(IDC_STATICPicture);//获得控件句柄
													   //CWnd* pWin = GetDlgItem(IDR_menu);
			CDC* pDC = pWin->GetDC();
			pDC->TextOut(start_pt.x, start_pt.y, box_str);

			//当左键抬起，将矩形框的对角点记录入临时字符串中

			ControlToImg(start_pt, org);
			ControlToImg(end_pt, org);//将起始点转换为图像坐标系

									  //输入方框编号
			str += (to_string(lbu_boxnum) + " ");
			//输入螺栓编号
			str += lbu_boltnum + " ";
			//输入方框起始点坐标
			str += (to_string(start_pt.x) + " ");
			str += (to_string(start_pt.y) + " ");
			str += (to_string(end_pt.x) + " ");
			str += (to_string(end_pt.y) + " ");
		}
		else
			DrawRect(start_pt, end_pt);

	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMFCApplication1Dlg::ResizeWindow(CRect &rect, int img_cols, int img_rows, float &scale)
{
	float precols = img_cols * 1.0;

	int rw = rect.Width();
	int rh = rect.Height();
	int origin_x = 0;
	int origin_y = 0;

	//固定h,w相应缩放
	//AfxMB_i(img_rows);
	img_cols = img_cols * rh / img_rows;
	img_rows = rh;
	scale = rh * 1.0 / img_rows;
	if (img_cols > rw)
	{
		//固定w,h相应缩放
		img_rows = img_rows * rw / img_cols;
		img_cols = rw;
		origin_y += (rh - img_rows) / 2;
	}
	else
		origin_x += (rw - img_cols) / 2;
	scale = img_cols / precols;
	CRect rect2(origin_x, origin_y, img_cols + origin_x, img_rows + origin_y);
	rect = rect2;
}
/*    */
void CMFCApplication1Dlg::ControlToImg(CPoint &point, Mat cti_img)
{
	CWnd* pWin = GetDlgItem(IDC_STATICPicture);
	CRect rect, rect2;
	pWin->GetWindowRect(&rect);
	/*CString rectstr;
	rectstr.Format(_T("%d,%d"), rect.Width(), rect.Height());
	AfxMessageBox(rectstr);*/
	rect2 = rect;
	float s = 0;
	ResizeWindow(rect2, cti_img.cols, cti_img.rows, s);//当图片实际尺寸小于控件尺寸时，缩放比例s>1
	CPoint imgOrigin;//图片原点在控件坐标系中的坐标
	imgOrigin.x = (rect.Width() - rect2.Width()) / 2;
	imgOrigin.y = (rect.Height() - rect2.Height()) / 2;
	/*CString rectstr;
	rectstr.Format(_T("%d,%d"), imgOrigin.x, imgOrigin.y);
	AfxMessageBox(rectstr);*/
	CPoint ctrpt = point;
	point.y = (ctrpt.y - imgOrigin.y) / s;
	point.x = (ctrpt.x - imgOrigin.x) / s;
}

void CMFCApplication1Dlg::DrawRect(CPoint ptUL, CPoint ptLR)
{
	//AfxMessageBox(_T("here"));

	CWnd* pWin = GetDlgItem(IDC_STATICPicture);//获得控件句柄
	CDC* pDC = pWin->GetDC();//得到该控件的画布，在画布上绘制

							 //CDC* pDC = this->GetDC();//在全局绘制

	CRect rect;
	pWin->GetClientRect(rect);//获得控件区域
							  /*pWin->ClientToScreen(&ptUL);
							  pWin->ClientToScreen(&ptLR);*/
	CRgn rgn;
	int h = rect.Height();
	int w = rect.Width();
	rgn.CreateRectRgn(15, 15, w + 15, h + 15);
	//pDC->SelectClipRgn(&rgn);//限制在rgn区域内画图


	// 设置透明画刷
	CBrush brush;
	//brush.CreateSolidBrush(PS_SOLID, 2, RGB(0, 255, 0));
	CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush *pOldBrush = pDC->SelectObject(pBrush);

	if (1)
	{
		pDC->SetROP2(R2_NOT);
		//pDC->Rectangle(rect);
		pDC->Rectangle(ptUL.x, ptUL.y, ptLR.x, ptLR.y);
	}
	else
	{
		CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
		CPen *pOldPen = pDC->SelectObject(&pen);
		pDC->Rectangle(ptUL.x, ptUL.y, ptLR.x, ptLR.y);
	}
	pWin->Invalidate();
	//pWin->UpdateWindow();
	ReleaseDC(pDC);

}


/*
//创建线程函数
HANDLE CreateThread(
LPSECURITY_ATTRIBUTES lpThreadAttributes,//指向线程安全属性，一般置为NULL
DWORD dwStackSize,//线程堆栈深度，一般置为0
//线程起始地址即要执行的函数地址，一般为"(LPTHREAD_START_ROUTINE)ThreadFunc"
//其中ThreadFunc为函数名
LPTHREAD_START_ROUTINE lpStartAddress,
LPVOID lpParameter,//线程函数的参数
//控制线程创建的附加标志，可以取两种值。如果该参数为0，线程在被创建后就会立即开始执行
//如果为CREATE_SUSPENDED,则系统产生线程后，该线程处于挂起状态，并不马上执行，直至函数 ResumeThread被调用
DWORD dwCreationFlags,
LPDWORD lpThreadId//返回所创建线程的ID
);*/

void CMFCApplication1Dlg::tag(LPVOID args)
{
	//AfxMessageBox(_T("here"));
	fileData* tagfdataptr = (fileData*)args;
	//CMFCApplication1Dlg* this1 = (CMFCApplication1Dlg*)args;
	//vector<CString>* tagfileNameptr = (vector<CString>*)args;
	//vector<CString> tagfileName = *tagfileNameptr;
	CImage tagimage;
	Mat tagorg;
	MatImageConversion mic;
	int fileNamesize = (tagfdataptr->filePath).size();
	//int fileNamesize = tagfileName.size();
	for (int i = 0; i < fileNamesize; i++)
	{

		if (!tagimage.IsNull())
		{
			tagimage.Destroy();
		}
		tagimage.Load((tagfdataptr->filePath)[i]);
		mic.CImageToMat(tagimage, tagorg);
		tagfdataptr->tagthis->org = tagorg;
		tagfdataptr->tagthis->ClearControl();
		tagfdataptr->tagthis->ShowToControl(tagorg);

		whileflag = true;
		tagfdataptr->tagthis->OnMuGenerate();

		while (whileflag) { waitKey(1); }
		tagfdataptr->tagthis->UpdateWindow();
		//AfxMessageBox(_T("dfs"));
	}
	AfxMessageBox(_T("已到最后一张"));
	return (void)NULL;
}

void CMFCApplication1Dlg::OnMuChoosefile()
{
	// TODO: 在此添加命令处理程序代码
	ShowRotationCtrl(false);
	//打开一个文件夹
	CString FoldPathName;
	//打开一个文件夹---方法一
	CFolderPickerDialog fd(NULL, 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		//清空str
		str.clear();

		//CString des;
		FoldPathName = fd.GetPathName();
		FoldName = fd.GetFileName();
		//AfxMessageBox(FoldName);

		//打开一个文件夹---方法二
		//TCHAR	szFolderPath[MAX_PATH] = { 0 };
		//BROWSEINFO sInfo;
		//ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		//sInfo.pidlRoot = 0;
		//sInfo.lpszTitle = _T("请选择一个文件夹：");
		//sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
		//sInfo.lpfn = NULL;
		//LPITEMIDLIST lpidlBrowse = SHBrowseForFolder(&sInfo);
		//if (lpidlBrowse != NULL)
		//{
		//	// 取得文件夹名
		//	if (SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		//	{
		//		FoldPathName = szFolderPath;
		//	}
		//	CoTaskMemFree(lpidlBrowse);
		//}
		//AfxMessageBox(FoldPathName);

		//遍历文件夹下的文件
		CFileFind finder;
		BOOL res = finder.FindFile(FoldPathName + "\\*.*");
		//vector<CString> fileName;

		//bool whileflag = false;
		while (res)
		{

			res = finder.FindNextFile();
			//AfxMessageBox(_T("here"));
			//如果不是子文件夹且不是"."
			if (!(finder.IsDirectory()) && (!finder.IsDots()))
			{
				filePath.push_back(finder.GetFilePath());
				fileName.push_back(finder.GetFileName());
				//AfxMessageBox(fileName);
				/*
				//线程调用函数一，不阻塞
				//ResumeThread(threadID);
				//线程调用函数二，阻塞，即当子线程完成，主线程才继续
				WaitForSingleObject(threadID, INFINITE);
				CloseHandle(threadID);*/
			}
		}

		fdata.filePath = filePath;
		fdata.tagthis = this;

		LPDWORD lpThreadId = NULL;
		threadhandle = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(tag), &fdata, 0, lpThreadId);
		//ResumeThread(threadID);

		chooseflag = true;
		UpdateWindow();
		//该语句在OnInitDialog()中首次出现，同时应在每个使用CFileDialog打开窗口的代码之后出现，
		//否则窗口会变灰。在使用了AfxMessageBox的代码段中可以不使用该语句。
		//SetMenu(&menu);
		//if (chooseflag == true && generateflag == false)
		//{
		//	//AfxMessageBox(_T("dfs"));
		//	m_startCorner = false;
		//}
		m_startCorner = false;
		ShowSubmitCtrl(true);

	}
	SetMenu(&menu);

}

void CMFCApplication1Dlg::OnMuGenerate()
{
	// TODO: 在此添加命令处理程序代码
	//生成一个string字符串，存到字符串中，最后再存到TXT中
	//ofstream fout("E:\\VSFile\\tempTXT.txt");
	//将fout传入imgdata是否可行？无法传入
	generateflag = true;
	//打开绘制角点的开关
	m_startCorner = true;
	//生成特征点与特征描述符
	imgdata.get_ImgData(org, fileName[file_num]);
	str += "\n";
	str += "/*****************start*************************/";
	str += "\n";
	str += imgdata.get_str();
	//AfxMessageBox(_T("已生成。请按照顺序框选目标。"));
	file_num++;
	if (chooseflag == true && generateflag == true)
	{

		//AfxMB_i(file_num);
		chooseflag = false;
		generateflag = false;
		box_num = 0;

	}
}

void CMFCApplication1Dlg::OnButtonSubmit()
{
	whileflag = false;

}

void CMFCApplication1Dlg::OnMuSave()
{
	// TODO: 在此添加命令处理程序代码
	/*CString boxstr;
	boxstr.Format(_T("%d"), box_num);
	AfxMessageBox(boxstr);*/
	//点击保存，前面的开关关闭
	chooseflag = false;
	generateflag = false;
	m_startCorner = false;

	CString FilePathName;
	//CFileDialog filedlg(FALSE, NULL, NULL, 0, _T("TXT (*.txt)|*.txt||"), NULL);

	//LPCTSTR lp=_T("txt");
	CFileDialog filedlg(FALSE, _T("txt"), FoldName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("TXT (*.txt)|*.txt||"));
	filedlg.m_ofn.lpstrTitle = _T("打开");
	if (filedlg.DoModal() == IDOK)
	{
		FilePathName = filedlg.GetPathName();
		//将临时字符串str存入TXT
		//AfxMessageBox(FilePathName);

		ofstream fout(FilePathName);
		//AfxMB_i(file_num);
		fout << file_num << endl;//存入文件总数
		file_num = 0;
		fout << box_num;//存入矩形框数（每个文件中都有相同数量的矩形框）

		int str_size = str.size();
		for (int i = 0; i < str_size; i++)
		{
			fout << str[i];
			if (i == str_size - 1)
				AfxMessageBox(_T("已保存"));
		}
		fout.close();

		str.clear();//清空字符串
		ShowSubmitCtrl(false);
		Invalidate();
	}
	ShowSubmitCtrl(false);
	ShowRotationCtrl(false);
	SetMenu(&menu);
	UpdateWindow();
}

void CMFCApplication1Dlg::OnMuVideo()
{
	// TODO: 在此添加命令处理程序代码
	//点击保存，前面的开关关闭
	ClearControl();
	ShowRotationCtrl(false);

	chooseflag = false;
	generateflag = false;
	m_startCorner = false;

	//string FilePathName="";

	//CFileDialog filedlg(true); //显示打开对话框
	CFileDialog filedlg(TRUE, NULL, NULL, 0, _T("MP4 (*.mp4)|*.mp4|FLV (*.flv)|*.flv|AVI (*.avi)|*.avi||"), NULL);
	filedlg.m_ofn.lpstrTitle = _T("打开");
	//CFile f;
	//CFileException e;
	//VideoCapture cap;

	if (filedlg.DoModal() == IDOK)
	{
		FilePathName = CT2A(filedlg.GetPathName().GetBuffer());
		VideoCapture cap;
		cap.open(FilePathName);
		cap >> srcImage;
		ShowToControl(srcImage);
		cap.~VideoCapture();
		//FilePathName.clear();
		//Invalidate();
		ShowRotationCtrl(true);
	}
	if (srcImage.empty())
		AfxMessageBox(_T("视频读取失败"));
	ShowSubmitCtrl(false);
	SetMenu(&menu);
}

//void tracking(LPVOID args)
//{
//	CMFCApplication1Dlg* dlg = (CMFCApplication1Dlg*)args;
//	while(true)
//	{ 
//	tracker->update(srcImage, rect2d);
//	rectangle(srcImage, rect2d, Scalar(255, 0, 123), 2, 1);
//	dlg->ShowToControl(srcImage);
//	waitKey(30);
//	}
//}

int threadcounter = 0;
void CMFCApplication1Dlg::updaterect2d(LPVOID args)
{
	threadcounter++;
	CMFCApplication1Dlg* dlg = (CMFCApplication1Dlg*)args;
	while (true)
	{
		while (dlg->updateflag)
		{
			//将查询图像与每个模板图像都进行匹配，匹配点数最多的作为正确匹配
			//将模板图像与查询图像进行匹配，得到透视变换矩阵H
			dlg->H = dlg->get_H(dlg->V_Data, dlg->cur_ol_corner);

			//框出目标物体
			dlg->getbox(dlg->V_Data, dlg->targetNum, dlg->pre_corner, dlg->cur_ol_corner);
			//if ((dlg->rect2d.width > 4) && (dlg->rect2d.height > 4))
			{
				dlg->tracker->init(dlg->srcImage, dlg->rect2d);
			}

		}
	}
	//dlg->showbnicon();
	threadcounter--;
	AfxMessageBox(_T("0"));
}

void CMFCApplication1Dlg::OnMuMatch()
{
	// TODO: 在此添加命令处理程序代码
	//点击保存，前面的开关关闭
	chooseflag = false;
	generateflag = false;
	m_startCorner = false;

	int b_num = 0;
	ReadImgData(b_num);//读取数据，得到V_Data
	ChooseTarget dlg;
	dlg.getboxnum(b_num);
	dlg.DoModal();
	CString editstr = dlg.geteditstr();
	targetNum = _ttoi(editstr);

	ClearControl();
	//ShowSubmitCtrl(false);
	//showbnicon();
	/********************以下为匹配阶段***************************/
	VideoCapture* cap = new VideoCapture();
	cap->open(FilePathName);
	int cap_count = cap->get(CV_CAP_PROP_FRAME_COUNT);//得到视频总帧数
	double caprate = cap->get(CV_CAP_PROP_FPS);//读取帧率
	int frame_num = 0;

	//Ptr<Tracker> tracker = TrackerMIL::create();
	//double rect_x = 0.0, rect_y = 0.0, rect_w = 0.0, rect_h = 0.0;
	//Rect2d rect2d;
	bool firstflag = true;
	HANDLE trackThreadhandle;
	//cap_count = 100;
	for (int i = 0; i<cap_count; i++)
	{
		//查询图像srcImage
		*cap >> srcImage;
		//cap.read(capframe);
		if (srcImage.empty())
			AfxMessageBox(_T("视频读取失败"));
		//imshow("【目标追踪】", capframe);
		//cout << frame_num << endl;
		//旋转图像
		//rotate(srcImage2, 270);
		//srcImage2.copyTo(org);
		//每5帧更新一次box
		if (frame_num % 2 == 0)
		{
			//将查询图像与每个模板图像都进行匹配，匹配点数最多的作为正确匹配
			//将模板图像与查询图像进行匹配，得到透视变换矩阵H
			H = get_H(V_Data, cur_ol_corner);

			//框出目标物体
			getbox(V_Data, targetNum, pre_corner, cur_ol_corner);

			waitKey(1);
		}

		//每帧都画框，如果更新了就画新框，没更新就画旧框
		drawbox(pre_corner);
		//if (frame_num % 50 == 0)
		//{
		//	updateflag = true;
		//}
		//else
		//	updateflag = false;
		//if (firstflag)
		//{
		//	H = get_H(V_Data, cur_ol_corner);
		//	//从getbox函数中得到rect2d
		//	getbox(V_Data, targetNum, pre_corner, cur_ol_corner);
		//	//if ((rect2d.width > 4)&&(rect2d.height > 4))
		//	{
		//		tracker->init(srcImage, rect2d);
		//		tracker->update(srcImage, rect2d);
		//		rectangle(srcImage, rect2d, Scalar(255, 0, 123), 2, 1);
		//	}
		//	
		//	ShowToControl(srcImage);
		//	waitKey(caprate);

		//	//LPDWORD trackThreadId = NULL;
		//	trackThreadhandle = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(updaterect2d), this, 0, NULL);
		//	firstflag = false;
		//}
		////每帧都画框，如果更新了就画新框，没更新就画旧框
		////drawbox(pre_corner);
		////if ((rect2d.width > 4) && (rect2d.height > 4))
		//{
		//	tracker->update(srcImage, rect2d);
		//	rectangle(srcImage, rect2d, Scalar(255, 0, 123), 2, 1);
		//}
		//

		//显示SRCImage到窗口
		//ShowToControl(srcImage);
		imshow("df", srcImage);
		waitKey(caprate);
		frame_num++;
	}
	delete(cap);
	WaitForSingleObject(trackThreadhandle, INFINITE);
	CloseHandle(trackThreadhandle);
	/*************************************************************/
}

void CMFCApplication1Dlg::ShowToControl(Mat stc_img)
{
	desImage = stc_img;
	if (rotationflag != 0)
		rotation(stc_img, desImage);
	CDC* pDC = GetDlgItem(IDC_STATICPicture)->GetDC();
	HDC hdc = pDC->GetSafeHdc();
	CRect rect;
	// 矩形类
	GetDlgItem(IDC_STATICPicture)->GetClientRect(&rect); //获取box1客户区
	IplImage *frame = NULL;
	IplImage frametemp;
	CvvImage cimg;
	CRect rect2 = rect;
	int w = desImage.cols;
	int h = desImage.rows;
	float s = 0;//缩放比例，此处无用
	ResizeWindow(rect2, w, h, s);//使视频以适合控件大小的形式显示

	frametemp = desImage;
	frame = cvCloneImage(&frametemp);
	cimg.CopyOf(frame, frame->nChannels);

	cimg.DrawToHDC(hdc, &rect2);//显示到控件
								//float ratio = cvGetCaptureProperty(capture, CV_CAP_PROP_POS_AVI_RATIO);//读取该帧在视频中的相对位置



}

void CMFCApplication1Dlg::ClearControl()
{
	//CMFCApplication1Dlg *pMMD = (CMFCApplication1Dlg*)AfxGetMainWnd();     //获取主窗口指针
	//CWnd* pMWin = GetDlgItem(IDD_MFCAPPLICATION1_DIALOG);
	CWnd* pWin = GetDlgItem(IDC_STATICPicture);
	CDC *pDC = pWin->GetDC();
	CRect rect;
	pWin->GetClientRect(&rect);
	//CStatic *pStatic = (CStatic *)GetDlgItem(IDC_STATICPicture);
	//GetDlgItem(IDC_STATICPicture)->SetBitmap(NULL);
	CBrush br(0xffffff);
	pDC->FillRect(rect, &br);
	//this->RedrawWindow();

	//showbnicon();
}

void CMFCApplication1Dlg::ReadImgData(int &b_num)
{
	//先清空V_Data
	if (!V_Data.empty())
	{
		V_Data.clear();
	}
	Img_Data temp_data;
	CString FilePathName;
	CFileDialog filedlg(TRUE, NULL, NULL, 0, _T("TXT (*.txt)|*.txt||"), NULL);
	filedlg.m_ofn.lpstrTitle = _T("打开");
	if (filedlg.DoModal() == IDOK)
	{
		FilePathName = filedlg.GetPathName();

		fstream fin(FilePathName);
		if (!fin.is_open())
			AfxMessageBox(_T("读取失败"));
		int f_num = 0;
		fin >> f_num;//读取文件总数
		fin >> b_num;//读取矩形框数
		string startflag, startstr;
		fin >> startflag;
		startstr = "/*****************start*************************/";
		//if (fin>>startflag == startstr);
		for (int i = 0; i<f_num; i++)
		{
			//读取图片编号
			fin >> temp_data.Img_code;

			//读取特征点集

			fin >> temp_data.kp_num;
			KeyPoint kptmp;

			CString intstr;
			intstr.Format(_T("%d"), temp_data.kp_num);
			//AfxMessageBox(intstr);

			for (int i = 0; i < temp_data.kp_num; i++)
			{
				fin >> kptmp.pt.x >> kptmp.pt.y >> kptmp.size >> kptmp.angle >> kptmp.response >> kptmp.octave >> kptmp.class_id;
				temp_data.ol_keypoints.push_back(kptmp);
			}

			//cout << "特征点：" << temp_data.ol_keypoints.size() << endl;
			//读取描述符
			fin >> temp_data.r >> temp_data.c;

			intstr.Format(_T("%d   %d"), temp_data.r, temp_data.c);
			//AfxMessageBox(intstr);

			Mat ol_descriptors(temp_data.r, temp_data.c, CV_32F);
			float destmp = 0.0;
			for (int i = 0; i < temp_data.r; i++)
				for (int j = 0; j < temp_data.c; j++)
				{
					fin >> destmp;
					ol_descriptors.at<float>(i, j) = destmp;
				}
			temp_data.ol_descriptors = ol_descriptors;

			//读取角点	
			Point cortmp;
			Box boxtmp;
			int whileint = 0;

			//for(int i=0;i<b_num;i++)

			//6个数一组。第一个数为boxnum，二为boltnum，三为start_pt.x，四为start_pt.y
			//五为end_pt.x，六为end_pt.y
			int numIn6 = 0;
			while (!fin.eof())
			{
				fin >> startflag;
				if (startflag == startstr)
					break;

				numIn6 = whileint % 6;
				switch (numIn6)
				{
					//atoi是string转int，atof是string转float
				case 0:
					boxtmp.setboxnum(atoi(startflag.c_str()));
					break;
				case 1:
					boxtmp.setboltnum(startflag);
					break;
				case 2:
					boxtmp.setstartpt_x(atof(startflag.c_str()));
					break;
				case 3:
					boxtmp.setstartpt_y(atof(startflag.c_str()));
					break;
				case 4:
					boxtmp.setendpt_x(atof(startflag.c_str()));
					break;
				case 5:
					boxtmp.setendpt_y(atof(startflag.c_str()));
					temp_data.ol_corners.push_back(boxtmp);
					break;

				}

				whileint++;
			}
			//每次都把最后一个角点读两遍，所以读完之后在此处把最后那个重复的点删掉
			//temp_data.ol_corners.pop_back();
			//cout << "角点：" << temp_data.ol_corners.size() << endl;

			V_Data.push_back(temp_data);
			//如果不清空，角点会累积到下一个V_Data的成员中去
			temp_data.ol_corners.clear();
			temp_data.ol_keypoints.clear();


			//AfxMessageBox(_T("here"));


		}
		fin.clear();
		fin.close();
		//AfxMessageBox(_T("读取完毕"));
		/*int is1 = V_Data[0].ol_corners.size();
		CString stris1;
		stris1.Format(_T("%d"), is1);
		AfxMessageBox(stris1);*/
	}
}

Mat CMFCApplication1Dlg::get_H(vector<Img_Data> v_data, vector<Box> &cur_ol_corner)
{
	//得到在线数据
	vector<KeyPoint> keypoints_scene;
	Mat descriptors_scene;
	get_online_data(keypoints_scene, descriptors_scene);
	//每一个都匹配，返回匹配点数
	Img_Data best_match = v_data[0];
	int match_num = 0, cur_num = 0;
	vector<Img_Data>::iterator it;
	vector<vector<DMatch>> cur_matches;
	vector<Point2f> obj, cur_obj;
	vector<Point2f> scene, cur_scene;
	match_num = get_match_num(v_data[0].ol_descriptors, descriptors_scene, v_data[0], keypoints_scene,
		obj, scene);
	//cout << ":" << match_num << endl;

	for (it = v_data.begin(); it != v_data.end(); it++)
	{
		cur_num = get_match_num((*it).ol_descriptors, descriptors_scene, *it, keypoints_scene,
			cur_obj, cur_scene);
		//cout << "模板编码:" << (*it).Img_code << "匹配数：" << cur_num << endl;
		if (cur_num > match_num)
		{
			//cout << "cur_matches:" << cur_matches.size() << endl;
			best_match = (*it);
			obj = cur_obj;
			scene = cur_scene;

		}

	}
	//cout << "最佳匹配的模板编码：" << best_match.Img_code << endl;
	cur_ol_corner = best_match.ol_corners;
	vector<unsigned char> listpoints;
	H = findHomography(obj, scene, CV_RANSAC, 3, listpoints);//计算透视变换 
	return H;
}

void CMFCApplication1Dlg::get_online_data(vector<KeyPoint> &keypoints_scene, Mat &descriptors_scene)
{
	//【2】使用SURF算子检测关键点  
	//cv::SurfFeatureDetector surf(300);
	Ptr<Feature2D> surf = SURF::create(300);
	//vector<KeyPoint> keypoints_scene;//vector模板类，存放任意类型的动态数组  

	//【3】调用detect函数检测出SURF特征关键点，保存在vector容器中  
	surf->detect(srcImage, keypoints_scene);

	//【4】计算描述符（特征向量） 
	//cv::SurfDescriptorExtractor surfDesc;
	//Mat descriptors_scene;
	surf->compute(srcImage, keypoints_scene, descriptors_scene);
}

int CMFCApplication1Dlg::get_match_num(Mat des_template, Mat des_query, Img_Data best_match, vector<KeyPoint> keypoints_scene,
	vector<Point2f> &obj, vector<Point2f> &scene)
{
	//使用FLANN匹配算法中的knn匹配器进行匹配 
	FlannBasedMatcher matcher;
	vector<vector<DMatch>> matches;
	matcher.knnMatch(des_template, des_query, matches, 2);

	//定义两个局部变量，从匹配成功的匹配对中获取关键点
	DMatch bettermatch, goodmatch;
	vector<DMatch> bestmatch;
	double matchRatio = 0.0;
	for (unsigned int i = 0; i < matches.size(); i++)
	{
		bettermatch = matches[i][0];
		goodmatch = matches[i][1];
		matchRatio = (1.0*bettermatch.distance) / goodmatch.distance;
		if (matchRatio < 0.8)
		{
			bestmatch.push_back(bettermatch);
			//cout << "---"<<best_match.ol_keypoints.size() << "   " << bettermatch.queryIdx << endl;
			obj.push_back(best_match.ol_keypoints[bettermatch.queryIdx].pt);
			scene.push_back(keypoints_scene[bettermatch.trainIdx].pt);
		}

	}

	return bestmatch.size();
}

void CMFCApplication1Dlg::drawbox(vector<Point2f> pre_corner)
{
	//跟踪
	//Ptr<Tracker> tracker = TrackerMIL::create();
	//Rect2d dbbox(pre_corner[0].x, pre_corner[0].y, pre_corner[2].x,pre_corner[2].y);
	////imshow("【目标追踪】", srcImage);
	//tracker->init(srcImage, dbbox);
	//rectangle(srcImage, dbbox, Scalar(255, 0, 123), 2, 1);
	//在目标图像上标出目标物体的位置
	line(srcImage, pre_corner[0], pre_corner[1], Scalar(255, 0, 123), 4);
	line(srcImage, pre_corner[1], pre_corner[2], Scalar(255, 0, 123), 4);
	line(srcImage, pre_corner[2], pre_corner[3], Scalar(255, 0, 123), 4);
	line(srcImage, pre_corner[3], pre_corner[0], Scalar(255, 0, 123), 4);

	//显示最终结果
	//imshow("【目标追踪】", srcImage);
	//waitKey(20);
}

void CMFCApplication1Dlg::getbox(vector<Img_Data> v_data, int box_i, vector<Point2f> &cur_corners, vector<Box> cur_ol_corner)
{
	vector<Box> ol_corners = cur_ol_corner;

	//取出编号为box_i的框，放入boxs
	vector<Point2f> boxs;
	int olcorners_size = ol_corners.size();
	Point2f start_pt(0, 0), end_pt(0, 0);
	for (int i = 0; i < olcorners_size; i++)
	{
		if (ol_corners[i].getboxnum() == box_i)
		{
			//AfxMessageBox(_T("here"));
			start_pt = ol_corners[i].getstartpt();
			end_pt = ol_corners[i].getendpt();
			boxs.push_back(start_pt);
			boxs.push_back(end_pt);
		}
	}
	//如果没有要找的目标，将boxs的大小置为0
	if (boxs.size() == 0)
	{
		boxs.push_back(start_pt);
		boxs.push_back(end_pt);
	}
	/*float is1 = box_i;
	CString stris1;
	stris1.Format(_T("%f"), is1);
	AfxMessageBox(stris1);*/
	////取出每个框
	//vector<vector<Point2f>> boxs;
	//vector<Point2f> boxtmp(2);
	//for (int i = 0; i < ol_corners.size(); i++)
	//{
	//	boxtmp[i % 2] = ol_corners[i];
	//	if (i % 2 == 1)
	//		boxs.push_back(boxtmp);

	//}
	//从待测图片中获取角点  
	vector<Point2f> trans_corners(2);
	//trans_corners = boxs[box_i];
	//进行透视变换  
	perspectiveTransform(boxs, trans_corners, H);
	boxs.clear();

	vector<Point2f> scene_corners(4);
	scene_corners[0] = trans_corners[0];
	scene_corners[2] = trans_corners[1];
	/*scene_corners[0].x -= 20;
	scene_corners[0].y -= 200;
	scene_corners[2].x -= 20;
	scene_corners[2].y -= 200;*/

	scene_corners[1].x = scene_corners[0].x;
	scene_corners[1].y = scene_corners[2].y;
	scene_corners[3].x = scene_corners[2].x;
	scene_corners[3].y = scene_corners[0].y;

	//如果面积大于阈值就更新
	int area = abs((scene_corners[0].x - scene_corners[2].x)*(scene_corners[0].y - scene_corners[2].y));
	//cout << "-------" << area << endl;
	/*if (area > 3000 && area<50000)
	{
	cur_corners = scene_corners;
	}*/
	//经过
	cur_corners = scene_corners;

	double rect_x = cur_corners[1].x;
	double rect_y = cur_corners[1].y;
	double rect_w = abs(cur_corners[2].x - cur_corners[0].x);
	double rect_h = abs(cur_corners[2].y - cur_corners[0].y);
	Rect2d rect2d_update(rect_x, rect_y, rect_w, rect_h);
	//当boxs大小为0时，rect2d的大小也为0
	rect2d = rect2d_update;
}

void CMFCApplication1Dlg::AfxMB_i(int i)
{
	CString cstr_i;
	cstr_i.Format(_T("%d"), i);
	AfxMessageBox(cstr_i);
}

void CMFCApplication1Dlg::ShowSubmitCtrl(bool sscflag)
{
	CEdit *edit = (CEdit*)GetDlgItem(IDC_BUTTONSubmit);

	if (sscflag)
		edit->EnableWindow(TRUE); //显示该控件

	else
	{
		edit->EnableWindow(SW_HIDE);//隐藏该控件

	}
}

void CMFCApplication1Dlg::ShowRotationCtrl(bool srcflag)
{
	CEdit *edit1 = (CEdit*)GetDlgItem(IDC_BUTTON1);
	CEdit *edit2 = (CEdit*)GetDlgItem(IDC_BUTTON2);
	if (srcflag)
	{
		edit1->EnableWindow(TRUE); //显示该控件
		edit2->EnableWindow(TRUE); //显示该控件
		showbnicon();
	}
	else
	{
		rotationflag = 0;
		edit1->EnableWindow(SW_HIDE);//隐藏该控件
		edit2->EnableWindow(SW_HIDE);//隐藏该控件
		disablebnicon();
	}


}

void CMFCApplication1Dlg::OnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox(_T("已隐藏"));
	rotationflag++;
	rotationflag = rotationflag % 4;

	//rotation(srcImage, desImage);

	ClearControl();
	ShowRotationCtrl(true);
	ShowToControl(srcImage);
}

void CMFCApplication1Dlg::OnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	rotationflag--;
	rotationflag = rotationflag % 4;

	//rotation(srcImage, desImage);

	ClearControl();
	ShowRotationCtrl(true);
	ShowToControl(srcImage);
}

void CMFCApplication1Dlg::rotation(Mat srcImg, Mat &desImg)
{
	int angles = 90 * rotationflag;
	cv::Point2f center(srcImg.cols / 2, srcImg.rows / 2);
	cv::Mat rot = cv::getRotationMatrix2D(center, angles, 1);
	cv::Rect bbox = cv::RotatedRect(center, srcImg.size(), angles).boundingRect();

	rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
	rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;

	//Mat desImage;
	cv::warpAffine(srcImg, desImg, rot, bbox.size());


}

void CMFCApplication1Dlg::showbnicon()
{
	CWnd* pwin1 = GetDlgItem(IDC_BUTTON1);
	CDC* pdc1 = pwin1->GetDC();
	HDC hdc1 = pdc1->GetSafeHdc();
	CRect rect1;
	pwin1->GetClientRect(&rect1);
	Mat icon1 = imread("E:\\downloads\\counterclockwise.png");
	if (icon1.empty())
		AfxMessageBox(_T("Icon read failed!"));
	//imshow("dfs", icon);
	IplImage frametemp1 = icon1;
	IplImage *frame1 = cvCloneImage(&frametemp1);
	CvvImage cimg1;
	cimg1.CopyOf(frame1, frame1->nChannels);
	cimg1.DrawToHDC(hdc1, &rect1);//显示到控件

	CWnd* pwin2 = GetDlgItem(IDC_BUTTON2);
	CDC* pdc2 = pwin2->GetDC();
	HDC hdc2 = pdc2->GetSafeHdc();
	CRect rect2;
	pwin2->GetClientRect(&rect2);
	Mat icon2 = imread("E:\\downloads\\clockwise.png");
	if (icon2.empty())
		AfxMessageBox(_T("Icon read failed!"));
	//imshow("dfs", icon);
	IplImage frametemp2 = icon2;
	IplImage *frame2 = cvCloneImage(&frametemp2);
	CvvImage cimg2;
	cimg2.CopyOf(frame2, frame2->nChannels);
	cimg2.DrawToHDC(hdc2, &rect2);//显示到控件
}

void CMFCApplication1Dlg::disablebnicon()
{
	CWnd* pwin1 = GetDlgItem(IDC_BUTTON1);
	CDC* pdc1 = pwin1->GetDC();
	HDC hdc1 = pdc1->GetSafeHdc();
	CRect rect1;
	pwin1->GetClientRect(&rect1);
	Mat icon1 = imread("E:\\downloads\\graycounterclockwise.png");
	if (icon1.empty())
		AfxMessageBox(_T("Icon read failed!"));
	//imshow("dfs", icon);
	IplImage frametemp1 = icon1;
	IplImage *frame1 = cvCloneImage(&frametemp1);
	CvvImage cimg1;
	cimg1.CopyOf(frame1, frame1->nChannels);
	cimg1.DrawToHDC(hdc1, &rect1);//显示到控件

	CWnd* pwin2 = GetDlgItem(IDC_BUTTON2);
	CDC* pdc2 = pwin2->GetDC();
	HDC hdc2 = pdc2->GetSafeHdc();
	CRect rect2;
	pwin2->GetClientRect(&rect2);
	Mat icon2 = imread("E:\\downloads\\grayclockwise.png");
	if (icon2.empty())
		AfxMessageBox(_T("Icon read failed!"));
	//imshow("dfs", icon);
	IplImage frametemp2 = icon2;
	IplImage *frame2 = cvCloneImage(&frametemp2);
	CvvImage cimg2;
	cimg2.CopyOf(frame2, frame2->nChannels);
	cimg2.DrawToHDC(hdc2, &rect2);//显示到控件
}


void CMFCApplication1Dlg::OnMuTemplates()
{
	// TODO: 在此添加命令处理程序代码
	ClearControl();

	HANDLE omthandle = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(GetAerasize), this, 0, NULL);

	//WaitForSingleObject(omthandle, INFINITE);
	//CloseHandle(omthandle);
}

void CMFCApplication1Dlg::GetAerasize(LPVOID args)
{
	CMFCApplication1Dlg* dlg = (CMFCApplication1Dlg*)args;
	VideoCapture cap;
	cap.open(dlg->FilePathName);

	CWnd* pWin = dlg->GetDlgItem(IDC_STATICPicture);
	CDC *pControlDC;
	pControlDC = pWin->GetDC();
	pControlDC->SelectStockObject(BLACK_BRUSH); //设置画刷
	dlg->templatesets.GetPDC(pControlDC);
	//dlg->templatesets.ReadData(dlg->FilePathName, dlg->V_Aerasize);
	dlg->templatesets.GetAerasize(cap, dlg->V_Aerasize);

	CString str;
	pControlDC->MoveTo(50, 600);//画线的开始位置
	pControlDC->LineTo(800, 600);
	pControlDC->MoveTo(50, 600);//画线的开始位置
	pControlDC->LineTo(50, 50);
	pControlDC->MoveTo(40, 60);//画线的开始位置
	pControlDC->LineTo(50, 50);
	pControlDC->LineTo(60, 60);
	pControlDC->MoveTo(790, 590);//画线的开始位置
	pControlDC->LineTo(800, 600);
	pControlDC->LineTo(790, 610);
	pControlDC->TextOut(800, 600, _T("x-帧号"));
	pControlDC->TextOut(20, 30, _T("y-影响区域"));
	CString strx, stry;
	int j = 0;
	for (int i = 0; i <= 26; i++)
	{
		j = i / 2;
		strx.Format(_T("%d"), j * 50);
		stry.Format(_T("%d"), i * 5);
		pControlDC->TextOut(50 + 50 * j, 610, strx);
		pControlDC->TextOut(20, 600 - 20 * i, stry);
	}

	int aerasize = dlg->V_Aerasize.size();
	int jiange = 750 / aerasize;
	//CPen pen(BS_SOLID, 1, RGB(255, 0, 0));
	//pControlDC->SelectObject(&pen); //设置画刷
	pControlDC->MoveTo(50, 600 - dlg->V_Aerasize[0] * 4);
	for (int i = 1; i < aerasize; i++)
	{
		pControlDC->LineTo(50 + i * jiange, 600 - dlg->V_Aerasize[i] * 4);
	}
	return (void)NULL;
}