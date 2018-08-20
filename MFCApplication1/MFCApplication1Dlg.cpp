
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//全局变量
//char str[] = "";//用于临时保存图像数据
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
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
	m_startRect = false;
	m_startCorner = false;
	chooseflag = false;
	generateflag = false;
	file_num = 0;
	box_num = 0;
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
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

//如果向对话框添加最小化按钮，则需要下面的代码来绘制该图标。 
//对于使用文档/视图模型的 MFC 应用程序，这将由框架自动完成。

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
		//AfxMessageBox(_T("here"));
		//CWnd* pWin = GetDlgItem(IDC_STATICPicture);//获得控件句柄 
		//CDC *pDc = pWin->GetDC();

		//CRect rect;
		//pWin->GetClientRect(&rect);//获得控件区域
		//int rw = rect.Width();
		//int rh = rect.Height();
		//if (!image.IsNull())
		//{
		//	//int h = image.GetHeight();
		//	//int w = image.GetWidth();
		//	//int origin_x = 0;
		//	//int origin_y = 0;
		//	//
		//	////固定h,w相应缩放
		//	//w = w * rh / h;
		//	//h = rh;
		//	//if (w > rw)
		//	//{
		//	//	//固定w,h相应缩放
		//	//	h = h * rw / w;
		//	//	w = rw;
		//	//	origin_y += (rh - h) / 2;
		//	//}
		//	//else
		//	//	origin_x += (rw - w) / 2;
		//	//CRect rect2(origin_x, origin_y, w+origin_x, h+ origin_y);
		//	//image.Draw(pDc->GetSafeHdc(), rect2);
		//	CRect rect2=rect;
		//	ResizeWindow(rect2, image.GetWidth(), image.GetHeight());
		//	image.Draw(pDc->GetSafeHdc(), rect2);
		//	//image.ReleaseDC();
		//}

		//this->ReleaseDC(pDc);
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

void CMFCApplication1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//AfxMessageBox(_T("左键按下"));
	//if (m_startCorner == true)AfxMessageBox(_T("true"));
	//else AfxMessageBox(_T("false"));
	//左键单击画点
	DrawEllipse(point, 2);

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
	//实时显示鼠标坐标
	//CString mouse_str,img_str;
	//mouse_str.Format(_T("[%d,%d] "), point.x, point.y);
	//CWnd* pWin = GetDlgItem(IDC_STATICPicture);//获得控件句柄
	////CWnd* pWin = GetDlgItem(IDR_menu);
	//CDC* pDC = pWin->GetDC();
	//pDC->TextOut(0, 0, mouse_str);
	//if (!org.empty())
	//{
	//	CPoint imgpt = point;
	//	ControlToImg(imgpt, org);
	//	img_str.Format(_T("[%d,%d] "), imgpt.x, imgpt.y);
	//	pDC->TextOut(100, 0, img_str);
	//}
	////UpdateWindow();
	//ReleaseDC(pDC);

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
	//if(chooseflag == true && generateflag == true)
	{
		m_startRect = false;
		box_num++;

		//显示序号
		CString box_str;
		box_str.Format(_T(" [%d] "), box_num);
		CWnd* pWin = GetDlgItem(IDC_STATICPicture);//获得控件句柄
		//CWnd* pWin = GetDlgItem(IDR_menu);
		CDC* pDC = pWin->GetDC();
		pDC->TextOut(start_pt.x, start_pt.y, box_str);

		//当左键抬起，将矩形框的对角点记录入临时字符串中
		ControlToImg(start_pt, org);
		ControlToImg(end_pt, org);//将起始点转换为图像坐标系

		str += (to_string(start_pt.x)+" ");
		str += (to_string(start_pt.y) + " ");
		str += (to_string(end_pt.x) + " ");
		str += (to_string(end_pt.y) + " ");
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

void CMFCApplication1Dlg::ControlToImg(CPoint &point, Mat cti_img)
{
	CWnd* pWin = GetDlgItem(IDC_STATICPicture);
	CRect rect,rect2;
	pWin->GetWindowRect(&rect);
	/*CString rectstr;
	rectstr.Format(_T("%d,%d"), rect.Width(), rect.Height());
	AfxMessageBox(rectstr);*/
	rect2 = rect;
	float s = 0;
	ResizeWindow(rect2, cti_img.cols, cti_img.rows, s);
	CPoint imgOrigin;//图片原点在控件坐标系中的坐标
	imgOrigin.x = (rect.Width() - rect2.Width()) / 2;
	imgOrigin.y = (rect.Height() - rect2.Height()) / 2;
	/*CString rectstr;
	rectstr.Format(_T("%d,%d"), cti_img.cols, cti_img.rows);
	AfxMessageBox(rectstr);*/
	CPoint ctrpt = point;
	point.y = (ctrpt.y - imgOrigin.y)/s;
	point.x = (ctrpt.x - imgOrigin.x)/s;
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
	rgn.CreateRectRgn(15,15,w+15,h+15);
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

void CMFCApplication1Dlg::OnMuChoosefile()
{
	// TODO: 在此添加命令处理程序代码
	CString FilePathName;

	//CFileDialog filedlg(true); //显示打开对话框All Files (*.*)|*.*|
	//CFileDialog filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, theApp.GetMainWnd()->GetWindow(GW_HWNDPREV), NULL);
	CFileDialog filedlg(TRUE, NULL, NULL, 0, _T("JPG (*.jpg)|*.jpg|BMP (*.bmp)|*.bmp||"), NULL);
	filedlg.m_ofn.lpstrTitle = _T("打开");
	CFile f;
	CFileException e;

	if (filedlg.DoModal() == IDOK)
	{
		//当image非空的时候，销毁image
		if (!image.IsNull())
		{
			image.Destroy();
		}
		image.Load(filedlg.GetPathName());
		MatImageConversion mic;
		mic.CImageToMat(image, org);
		ClearControl();
		ShowToControl(org);
		
		//测试图片的原点位置
		//Mat org2(org.size(), CV_8U,Scalar(0));
		////原点
		//for(int i=0;i<10;i++)
		//	for(int j=0;j<10;j++)
		//		org2.at<uchar>(i, j) = 255;
		////x坐标
		//for (int i = 100; i<110; i++)
		//	for (int j = 0; j<10; j++)
		//		org2.at<uchar>(i, j) = 150;
		////y坐标
		///*for (int i = 0; i<10; i++)
		//	for (int j = 100; j<110; j++)
		//		org2.at<uchar>(i, j) = 55;*/
		//经过测试，控件的原点位置在其左上角，向右为x坐标正方向，向下为y坐标正方向
		//图片的原点位置同样在其左上角，向下为x坐标正方向，向右为y坐标正方向
		//ShowToControl(org2);
		chooseflag = true;
		Invalidate();
		//SetFocus();
	}

	//该语句在OnInitDialog()中首次出现，同时应在每个使用CFileDialog打开窗口的代码之后出现，
	//否则窗口会变灰。在使用了AfxMessageBox的代码段中可以不使用该语句。
	SetMenu(&menu);

	if (chooseflag == true && generateflag == false)
	{
		//AfxMessageBox(_T("dfs"));
		m_startCorner = false;
	}
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
	imgdata.get_ImgData(org);
	str += "\n";
	str += "/*****************start*************************/";
	str += "\n";
	str += imgdata.get_str();
	AfxMessageBox(_T("已生成。请按照顺序框选目标。"));

	if (chooseflag == true && generateflag == true)
	{
		file_num++;
		chooseflag = false;
		generateflag = false;
		box_num = 0;
		
	}
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
	CFileDialog filedlg(FALSE, NULL, NULL, 0, _T("TXT (*.txt)|*.txt||"), NULL);
	filedlg.m_ofn.lpstrTitle = _T("打开");
	if (filedlg.DoModal() == IDOK)
	{
		FilePathName = filedlg.GetPathName();
		//将临时字符串str存入TXT
		fstream fout(FilePathName);
		fout << file_num << endl;//存入文件总数
		file_num = 0;
		fout << box_num ;//存入矩形框数（每个文件中都有相同数量的矩形框）

		int str_size = str.size();
		for (int i = 0; i < str_size; i++)
		{
			fout << str[i];
			if (i == str_size - 1)
				AfxMessageBox(_T("已保存"));
		}
		fout.close();
		str.clear();//清空字符串
		Invalidate();
	}
}

void CMFCApplication1Dlg::OnMuVideo()
{
	// TODO: 在此添加命令处理程序代码
	//点击保存，前面的开关关闭
	chooseflag = false;
	generateflag = false;
	m_startCorner = false;

	string FilePathName;

	//CFileDialog filedlg(true); //显示打开对话框
	CFileDialog filedlg(TRUE, NULL, NULL, 0, _T("MP4 (*.mp4)|*.mp4|FLV (*.flv)|*.flv|AVI (*.avi)|*.avi||"), NULL);
	filedlg.m_ofn.lpstrTitle = _T("打开");
	CFile f;
	CFileException e;
	//VideoCapture cap;

	if (filedlg.DoModal() == IDOK)
	{
		FilePathName = CT2A(filedlg.GetPathName().GetBuffer());
		cap.open(FilePathName);
		AfxMessageBox(_T("视频读取成功"));
		//Invalidate();
	}
	/*******************************************/
	//Mat capimg;
	///*while (cap.isOpened())
	//{
	//	cap >> capimg;
	//	imshow("dd", capimg);
	//	waitKey(20);
	//}*/
	//CDC* pDC = GetDlgItem(IDC_STATICPicture)->GetDC();
	//HDC hdc = pDC->GetSafeHdc();
	//CRect rect;
	//// 矩形类
	//GetDlgItem(IDC_STATICPicture)->GetClientRect(&rect); //获取box1客户区
	////CvCapture *capture = cvCreateFileCapture(FilePathName);  //读取视频
	//if (!cap.isOpened()) {
	//	printf("NO capture");    //读取不成功，则标识
	//							 //return 1;
	//};
	////double fps = cvGetCaptureProperty(cap, CV_CAP_PROP_FPS);   //读取视频的帧率
	////int vfps = 1000 / fps;                                        //计算每帧播放的时间
	////printf("%5.1f\t%5d\n", fps, vfps);
	//double frames = cap.get(CV_CAP_PROP_FRAME_COUNT);//读取视频中有多少帧
	//CString temp_value = _T("");   //temp_value用来处理float值
	////temp_value.Format(_T("%f"), frames);//固定格式
	////AfxMessageBox(temp_value);
	////printf("frames is %f\n", frames);
	////cvNamedWindow("example",CV_WINDOW_AUTOSIZE);                  //定义窗口
	//
	//IplImage *frame;
	//CvvImage cimg;
	//CRect rect2 = rect;
	//cap >> capimg;
	//int w = capimg.cols;
	//int h = capimg.rows;
	//ResizeWindow(rect2, w, h);//使视频以适合控件大小的形式显示
	//for(int i=0;i<frames;i++) 
	//{
	//	cap >> capimg;		
	//	frame = &capimg.operator IplImage();                         //抓取帧
	//	frame->origin = 0;
	//	cimg.CopyOf(frame, frame->nChannels);
	//	cimg.DrawToHDC(hdc, &rect2);//显示到控件
	//	//float ratio = cvGetCaptureProperty(capture, CV_CAP_PROP_POS_AVI_RATIO);     //读取该帧在视频中的相对位置
	//	//printf("%f\n", ratio);
	//	if (!frame)break;
	//	//cvShowImage("IDC_STATICPicture",frame);                          //显示

	//	//char c = cvWaitKey(vfps);
	//	//if (c == 27)break;
	//}
	//ReleaseDC(pDC);
	//cap.release();
	//cvReleaseCapture(&cap);
	//cvDestroyWindow("example");
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
	/*CString bnumstr;
	bnumstr.Format(_T("%d"), b_num);
	AfxMessageBox(bnumstr);*/
	//AfxMessageBox(_T("请选择识别目标"));
	ChooseTarget dlg;
	dlg.getboxnum(b_num);
	dlg.DoModal();
	CString editstr = dlg.geteditstr();
	targetNum = _ttoi(editstr)-1;
	/*if(targetNum==2)
	MessageBox(editstr, _T("程序运行结果"), MB_OK);*/
	
	ClearControl();
	/********************以下为匹配阶段***************************/
	int cap_count = cap.get(CV_CAP_PROP_FRAME_COUNT);//得到视频总帧数
	int frame_num = 0;
	vector<Point2f> pre_corner(4);
	vector<Point2f> cur_ol_corner;
	//cap >> srcImage;
	for (int i = 0; i<cap_count - 1; i++)
	{
		//查询图像srcImage
		cap >> srcImage;

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
		
		//imshow("【原图】", org);
		//显示SRCImage到窗口
		ShowToControl(srcImage);

		frame_num++;
		//waitKey(1000);
	}
	/*************************************************************/
}

void CMFCApplication1Dlg::ShowToControl(Mat stc_img)
{
	CDC* pDC = GetDlgItem(IDC_STATICPicture)->GetDC();
	HDC hdc = pDC->GetSafeHdc();
	CRect rect;
	// 矩形类
	GetDlgItem(IDC_STATICPicture)->GetClientRect(&rect); //获取box1客户区
	IplImage *frame;
	CvvImage cimg;
	CRect rect2 = rect;
	int w = stc_img.cols;
	int h = stc_img.rows;
	float s = 0;//缩放比例，此处无用
	ResizeWindow(rect2, w, h, s);//使视频以适合控件大小的形式显示
	//for (int i = 0; i<frames; i++)
	{
		frame = &stc_img.operator IplImage();                         //抓取帧
		frame->origin = 0;
		cimg.CopyOf(frame, frame->nChannels);
		cimg.DrawToHDC(hdc, &rect2);//显示到控件
									//float ratio = cvGetCaptureProperty(capture, CV_CAP_PROP_POS_AVI_RATIO);     //读取该帧在视频中的相对位置
									//printf("%f\n", ratio);
		//if (!frame)break;
		//cvShowImage("IDC_STATICPicture",frame);                          //显示

		//char c = cvWaitKey(vfps);
		//if (c == 27)break;
	}
}

void CMFCApplication1Dlg::ClearControl()
{
	CWnd* pWin = GetDlgItem(IDC_STATICPicture);
	CDC *pDC = pWin->GetDC();
	CRect rect;
	pWin->GetClientRect(&rect);
	//CStatic *pStatic = (CStatic *)GetDlgItem(IDC_STATICPicture);
	//GetDlgItem(IDC_STATICPicture)->SetBitmap(NULL);
	CBrush br(0xffffff);
	pDC->FillRect(rect, &br);
	//this->RedrawWindow();
}

void CMFCApplication1Dlg::ReadImgData(int &b_num)
{
	/*string fold = "E:\\learndata\\";
	CFileDialog dlg(TRUE);
	CString filename;
	if (dlg.DoModal() == IDOK)
		filename=dlg.GetPathName();*/
	//CFileFind ff;
	//String strDir = "E:\\learndata\\*.*";
	//CString cstrDir(strDir.c_str());//String转CString
	//BOOL res = ff.FindFile(cstrDir);
	//CString strFile;
	Img_Data temp_data;
	//String strfold = "E:\\learndata\\";
	//CString cstrfold(strfold.c_str());

	CString FilePathName;
	CFileDialog filedlg(FALSE, NULL, NULL, 0, _T("TXT (*.txt)|*.txt||"), NULL);
	filedlg.m_ofn.lpstrTitle = _T("打开");
	if (filedlg.DoModal() == IDOK)
	{
		FilePathName = filedlg.GetPathName();
	
		fstream fin(FilePathName);
		int f_num = 0;
		fin >> f_num;//读取文件总数
		fin >> b_num;//读取矩形框数
		string startflag,startstr;
		fin >> startflag;
		startstr = "/*****************start*************************/";
		//if (fin>>startflag == startstr);
		for(int i=0;i<f_num;i++)
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
			float destmp;
			for (int i = 0; i < temp_data.r; i++)
				for (int j = 0; j < temp_data.c; j++)
				{
					fin >> destmp;
					ol_descriptors.at<float>(i, j) = destmp;
				}
			temp_data.ol_descriptors = ol_descriptors;
			
			//读取角点	
			Point cortmp;
			int whileint = 0;

			while (fin>>startflag)
			{
				if (startflag == startstr)
					break;
				if (whileint % 2 == 1)
				{
					cortmp.y = atof(startflag.c_str());
					temp_data.ol_corners.push_back(cortmp);
				}
				else
					cortmp.x = atof(startflag.c_str());
							
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
		AfxMessageBox(_T("读取完毕"));
	}
}

Mat CMFCApplication1Dlg::get_H(vector<Img_Data> v_data, vector<Point2f> &cur_ol_corner)
{
	//得到在线数据
	vector<KeyPoint> keypoints_scene;
	Mat descriptors_scene;
	get_online_data(keypoints_scene, descriptors_scene);
	//每一个都匹配，返回匹配点数
	Img_Data best_match = v_data[0];
	int match_num, cur_num;
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
	cv::SurfFeatureDetector surf(300);
	//vector<KeyPoint> keypoints_scene;//vector模板类，存放任意类型的动态数组  

	//【3】调用detect函数检测出SURF特征关键点，保存在vector容器中  
	surf.detect(srcImage, keypoints_scene);

	//【4】计算描述符（特征向量） 
	cv::SurfDescriptorExtractor surfDesc;
	//Mat descriptors_scene;
	surfDesc.compute(srcImage, keypoints_scene, descriptors_scene);
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
	double matchRatio = 0;
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
	//在目标图像上标出目标物体的位置
	line(srcImage, pre_corner[0], pre_corner[1], Scalar(255, 0, 123), 4);
	line(srcImage, pre_corner[1], pre_corner[2], Scalar(255, 0, 123), 4);
	line(srcImage, pre_corner[2], pre_corner[3], Scalar(255, 0, 123), 4);
	line(srcImage, pre_corner[3], pre_corner[0], Scalar(255, 0, 123), 4);

	//显示最终结果
	//imshow("【目标追踪】", srcImage);
	waitKey(20);
}

void CMFCApplication1Dlg::getbox(vector<Img_Data> v_data, int box_i, vector<Point2f> &cur_corners, vector<Point2f> cur_ol_corner)
{
	vector<Point2f> ol_corners = cur_ol_corner;
	//取出每个框
	vector<vector<Point2f>> boxs;
	vector<Point2f> boxtmp(2);
	for (int i = 0; i < ol_corners.size(); i++)
	{
		boxtmp[i % 2] = ol_corners[i];
		if (i % 2 == 1)
			boxs.push_back(boxtmp);

	}
	//从待测图片中获取角点  
	vector<Point2f> trans_corners(2);
	//trans_corners = boxs[box_i];
	//进行透视变换  
	perspectiveTransform(boxs[box_i], trans_corners, H);

	vector<Point2f> scene_corners(4);
	scene_corners[0] = trans_corners[0];
	scene_corners[2] = trans_corners[1];

	scene_corners[1].x = scene_corners[0].x;
	scene_corners[1].y = scene_corners[2].y;
	scene_corners[3].x = scene_corners[2].x;
	scene_corners[3].y = scene_corners[0].y;

	//如果面积大于阈值就更新
	int area = abs((scene_corners[0].x - scene_corners[2].x)*(scene_corners[0].y - scene_corners[2].y));
	//cout << "-------" << area << endl;
	if (area > 3000 && area<50000)
	{
		cur_corners = scene_corners;
	}

}