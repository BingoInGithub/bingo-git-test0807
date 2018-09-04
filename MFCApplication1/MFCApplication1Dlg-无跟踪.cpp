
// MFCApplication1Dlg.cpp: ʵ���ļ�
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

//ȫ�ֱ���
//char str[] = "";//������ʱ����ͼ������
static Point pre_pt(-1, -1);//��ʼ����  
static Point cur_pt(-1, -1);//ʵʱ���� 
bool whileflag = false;


struct fileData {
	vector<CString> filePath;
	CMFCApplication1Dlg* tagthis;
}fdata;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
//CImage image;
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

														// ʵ��
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


// CMFCApplication1Dlg �Ի���



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


// CMFCApplication1Dlg ��Ϣ�������

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

									// TODO: �ڴ���Ӷ���ĳ�ʼ������
	ShowSubmitCtrl(false);
	ShowRotationCtrl(false);
	//CMenu menu;
	menu.LoadMenu(IDR_menu);
	SetMenu(&menu);

	/*HBITMAP hbitmap;
	hbitmap = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON3));
	((CButton *)GetDlgItem(IDC_BUTTON1))->SetBitmap(hbitmap);*/

	//::SetWindowText(AfxGetMainWnd()->GetSafeHwnd(), _T("Ҫ���õ�Ӧ�ù�������"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

//�����Ի��������С����ť������Ҫ����Ĵ��������Ƹ�ͼ�ꡣ 
//����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó����⽫�ɿ���Զ���ɡ�
//BOOL CMFCApplication1Dlg::OnEraseBkgnd(CDC* pDC)
//{
//	//return TRUE;
//	return CDialogEx::OnEraseBkgnd(pDC);
//}
void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
		//AfxMessageBox(_T("here"));
	}
	else
	{
		CDialogEx::OnPaint();
		ShowRotationCtrl(false);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//ShowSubmitCtrl(true);
	CDialogEx::OnRButtonDown(nFlags, point);
}
void CMFCApplication1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//AfxMessageBox(_T("�������"));
	//if (m_startCorner == true)AfxMessageBox(_T("true"));
	//else AfxMessageBox(_T("false"));
	//�����������
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
		//AfxMessageBox(_T("��ʼ���ƾ���"));
		start_pt = point;
		end_pt = point;
		m_startRect = true;

		pre_pt.x = point.x;
		pre_pt.y = point.y;

		//����Ϊʵ�鲿�֣��ڶ���DrawRect��������ȡ�������ִ�������൱�ڲ�ִ��
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//CString strx;
	//org = imread("E:\\Picture\\scene0830\\template-0.jpg");
	//ControlToImg(point, org);
	//strx.Format(_T("%d,%d"), point.x,point.y);
	////stry.Format(_T("%d"), point.y);
	//CWnd* pWin = GetDlgItem(IDC_STATICPicture);//��ÿؼ����
	//CDC* pDC = pWin->GetDC();//�õ��ÿؼ��Ļ������ڻ����ϻ���
	//ClearControl();
	//pDC->TextOut(point.x, point.y-15, strx);
	if (m_startRect == true && m_startCorner == true)
	{
		//��һ��DrawRect����һ�λ��Ŀ�������ڶ���DrawRect�����εĿ�
		DrawRect(start_pt, end_pt);
		DrawRect(start_pt, point);
		end_pt = point;

	}
	CDialogEx::OnMouseMove(nFlags, point);
}
void CMFCApplication1Dlg::drawrect(CPoint a, CPoint b)
{
	CWnd* pWin = GetDlgItem(IDC_STATICPicture);//��ÿؼ����
	CDC* pDC = pWin->GetDC();//�õ��ÿؼ��Ļ������ڻ����ϻ���
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (start_pt == end_pt)
		m_startRect = false;
	if ((m_startCorner == true) && (start_pt != end_pt))
	{
		m_startRect = false;

		//�������뷽����ź���˨���
		BoxNum boxnumdlg;
		boxnumdlg.DoModal();
		//�������
		int lbu_boxnum = 0;
		lbu_boxnum = boxnumdlg.getbnboxnum();
		if (lbu_boxnum > 0 && lbu_boxnum < 100)
		{
			//��˨���
			string lbu_boltnum = boxnumdlg.getbnboltnum();

			box_num = box_num > lbu_boxnum ? box_num : lbu_boxnum;


			//��ʾ�������
			CString box_str;
			box_str.Format(_T(" [%d] "), lbu_boxnum);
			CWnd* pWin = GetDlgItem(IDC_STATICPicture);//��ÿؼ����
													   //CWnd* pWin = GetDlgItem(IDR_menu);
			CDC* pDC = pWin->GetDC();
			pDC->TextOut(start_pt.x, start_pt.y, box_str);

			//�����̧�𣬽����ο�ĶԽǵ��¼����ʱ�ַ�����

			ControlToImg(start_pt, org);
			ControlToImg(end_pt, org);//����ʼ��ת��Ϊͼ������ϵ

									  //���뷽����
			str += (to_string(lbu_boxnum) + " ");
			//������˨���
			str += lbu_boltnum + " ";
			//���뷽����ʼ������
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

	//�̶�h,w��Ӧ����
	//AfxMB_i(img_rows);
	img_cols = img_cols * rh / img_rows;
	img_rows = rh;
	scale = rh * 1.0 / img_rows;
	if (img_cols > rw)
	{
		//�̶�w,h��Ӧ����
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
	ResizeWindow(rect2, cti_img.cols, cti_img.rows, s);//��ͼƬʵ�ʳߴ�С�ڿؼ��ߴ�ʱ�����ű���s>1
	CPoint imgOrigin;//ͼƬԭ���ڿؼ�����ϵ�е�����
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

	CWnd* pWin = GetDlgItem(IDC_STATICPicture);//��ÿؼ����
	CDC* pDC = pWin->GetDC();//�õ��ÿؼ��Ļ������ڻ����ϻ���

							 //CDC* pDC = this->GetDC();//��ȫ�ֻ���

	CRect rect;
	pWin->GetClientRect(rect);//��ÿؼ�����
							  /*pWin->ClientToScreen(&ptUL);
							  pWin->ClientToScreen(&ptLR);*/
	CRgn rgn;
	int h = rect.Height();
	int w = rect.Width();
	rgn.CreateRectRgn(15, 15, w + 15, h + 15);
	//pDC->SelectClipRgn(&rgn);//������rgn�����ڻ�ͼ


	// ����͸����ˢ
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
//�����̺߳���
HANDLE CreateThread(
LPSECURITY_ATTRIBUTES lpThreadAttributes,//ָ���̰߳�ȫ���ԣ�һ����ΪNULL
DWORD dwStackSize,//�̶߳�ջ��ȣ�һ����Ϊ0
//�߳���ʼ��ַ��Ҫִ�еĺ�����ַ��һ��Ϊ"(LPTHREAD_START_ROUTINE)ThreadFunc"
//����ThreadFuncΪ������
LPTHREAD_START_ROUTINE lpStartAddress,
LPVOID lpParameter,//�̺߳����Ĳ���
//�����̴߳����ĸ��ӱ�־������ȡ����ֵ������ò���Ϊ0���߳��ڱ�������ͻ�������ʼִ��
//���ΪCREATE_SUSPENDED,��ϵͳ�����̺߳󣬸��̴߳��ڹ���״̬����������ִ�У�ֱ������ ResumeThread������
DWORD dwCreationFlags,
LPDWORD lpThreadId//�����������̵߳�ID
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
	AfxMessageBox(_T("�ѵ����һ��"));
	return (void)NULL;
}

void CMFCApplication1Dlg::OnMuChoosefile()
{
	// TODO: �ڴ���������������
	ShowRotationCtrl(false);
	//��һ���ļ���
	CString FoldPathName;
	//��һ���ļ���---����һ
	CFolderPickerDialog fd(NULL, 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		//���str
		str.clear();

		//CString des;
		FoldPathName = fd.GetPathName();
		FoldName = fd.GetFileName();
		//AfxMessageBox(FoldName);

		//��һ���ļ���---������
		//TCHAR	szFolderPath[MAX_PATH] = { 0 };
		//BROWSEINFO sInfo;
		//ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		//sInfo.pidlRoot = 0;
		//sInfo.lpszTitle = _T("��ѡ��һ���ļ��У�");
		//sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
		//sInfo.lpfn = NULL;
		//LPITEMIDLIST lpidlBrowse = SHBrowseForFolder(&sInfo);
		//if (lpidlBrowse != NULL)
		//{
		//	// ȡ���ļ�����
		//	if (SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		//	{
		//		FoldPathName = szFolderPath;
		//	}
		//	CoTaskMemFree(lpidlBrowse);
		//}
		//AfxMessageBox(FoldPathName);

		//�����ļ����µ��ļ�
		CFileFind finder;
		BOOL res = finder.FindFile(FoldPathName + "\\*.*");
		//vector<CString> fileName;

		//bool whileflag = false;
		while (res)
		{

			res = finder.FindNextFile();
			//AfxMessageBox(_T("here"));
			//����������ļ����Ҳ���"."
			if (!(finder.IsDirectory()) && (!finder.IsDots()))
			{
				filePath.push_back(finder.GetFilePath());
				fileName.push_back(finder.GetFileName());
				//AfxMessageBox(fileName);
				/*
				//�̵߳��ú���һ��������
				//ResumeThread(threadID);
				//�̵߳��ú��������������������߳���ɣ����̲߳ż���
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
		//�������OnInitDialog()���״γ��֣�ͬʱӦ��ÿ��ʹ��CFileDialog�򿪴��ڵĴ���֮����֣�
		//���򴰿ڻ��ҡ���ʹ����AfxMessageBox�Ĵ�����п��Բ�ʹ�ø���䡣
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
	// TODO: �ڴ���������������
	//����һ��string�ַ������浽�ַ����У�����ٴ浽TXT��
	//ofstream fout("E:\\VSFile\\tempTXT.txt");
	//��fout����imgdata�Ƿ���У��޷�����
	generateflag = true;
	//�򿪻��ƽǵ�Ŀ���
	m_startCorner = true;
	//����������������������
	imgdata.get_ImgData(org, fileName[file_num]);
	str += "\n";
	str += "/*****************start*************************/";
	str += "\n";
	str += imgdata.get_str();
	//AfxMessageBox(_T("�����ɡ��밴��˳���ѡĿ�ꡣ"));
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
	// TODO: �ڴ���������������
	/*CString boxstr;
	boxstr.Format(_T("%d"), box_num);
	AfxMessageBox(boxstr);*/
	//������棬ǰ��Ŀ��عر�
	chooseflag = false;
	generateflag = false;
	m_startCorner = false;

	CString FilePathName;
	//CFileDialog filedlg(FALSE, NULL, NULL, 0, _T("TXT (*.txt)|*.txt||"), NULL);

	//LPCTSTR lp=_T("txt");
	CFileDialog filedlg(FALSE, _T("txt"), FoldName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("TXT (*.txt)|*.txt||"));
	filedlg.m_ofn.lpstrTitle = _T("��");
	if (filedlg.DoModal() == IDOK)
	{
		FilePathName = filedlg.GetPathName();
		//����ʱ�ַ���str����TXT
		//AfxMessageBox(FilePathName);

		ofstream fout(FilePathName);
		//AfxMB_i(file_num);
		fout << file_num << endl;//�����ļ�����
		file_num = 0;
		fout << box_num;//������ο�����ÿ���ļ��ж�����ͬ�����ľ��ο�

		int str_size = str.size();
		for (int i = 0; i < str_size; i++)
		{
			fout << str[i];
			if (i == str_size - 1)
				AfxMessageBox(_T("�ѱ���"));
		}
		fout.close();

		str.clear();//����ַ���
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
	// TODO: �ڴ���������������
	//������棬ǰ��Ŀ��عر�
	ClearControl();
	ShowRotationCtrl(false);

	chooseflag = false;
	generateflag = false;
	m_startCorner = false;

	//string FilePathName="";

	//CFileDialog filedlg(true); //��ʾ�򿪶Ի���
	CFileDialog filedlg(TRUE, NULL, NULL, 0, _T("MP4 (*.mp4)|*.mp4|FLV (*.flv)|*.flv|AVI (*.avi)|*.avi||"), NULL);
	filedlg.m_ofn.lpstrTitle = _T("��");
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
		AfxMessageBox(_T("��Ƶ��ȡʧ��"));
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
			//����ѯͼ����ÿ��ģ��ͼ�񶼽���ƥ�䣬ƥ�����������Ϊ��ȷƥ��
			//��ģ��ͼ�����ѯͼ�����ƥ�䣬�õ�͸�ӱ任����H
			dlg->H = dlg->get_H(dlg->V_Data, dlg->cur_ol_corner);

			//���Ŀ������
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
	// TODO: �ڴ���������������
	//������棬ǰ��Ŀ��عر�
	chooseflag = false;
	generateflag = false;
	m_startCorner = false;

	int b_num = 0;
	ReadImgData(b_num);//��ȡ���ݣ��õ�V_Data
	ChooseTarget dlg;
	dlg.getboxnum(b_num);
	dlg.DoModal();
	CString editstr = dlg.geteditstr();
	targetNum = _ttoi(editstr);

	ClearControl();
	//ShowSubmitCtrl(false);
	//showbnicon();
	/********************����Ϊƥ��׶�***************************/
	VideoCapture* cap = new VideoCapture();
	cap->open(FilePathName);
	int cap_count = cap->get(CV_CAP_PROP_FRAME_COUNT);//�õ���Ƶ��֡��
	double caprate = cap->get(CV_CAP_PROP_FPS);//��ȡ֡��
	int frame_num = 0;

	//Ptr<Tracker> tracker = TrackerMIL::create();
	//double rect_x = 0.0, rect_y = 0.0, rect_w = 0.0, rect_h = 0.0;
	//Rect2d rect2d;
	bool firstflag = true;
	HANDLE trackThreadhandle;
	//cap_count = 100;
	for (int i = 0; i<cap_count; i++)
	{
		//��ѯͼ��srcImage
		*cap >> srcImage;
		//cap.read(capframe);
		if (srcImage.empty())
			AfxMessageBox(_T("��Ƶ��ȡʧ��"));
		//imshow("��Ŀ��׷�١�", capframe);
		//cout << frame_num << endl;
		//��תͼ��
		//rotate(srcImage2, 270);
		//srcImage2.copyTo(org);
		//ÿ5֡����һ��box
		if (frame_num % 2 == 0)
		{
			//����ѯͼ����ÿ��ģ��ͼ�񶼽���ƥ�䣬ƥ�����������Ϊ��ȷƥ��
			//��ģ��ͼ�����ѯͼ�����ƥ�䣬�õ�͸�ӱ任����H
			H = get_H(V_Data, cur_ol_corner);

			//���Ŀ������
			getbox(V_Data, targetNum, pre_corner, cur_ol_corner);

			waitKey(1);
		}

		//ÿ֡��������������˾ͻ��¿�û���¾ͻ��ɿ�
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
		//	//��getbox�����еõ�rect2d
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
		////ÿ֡��������������˾ͻ��¿�û���¾ͻ��ɿ�
		////drawbox(pre_corner);
		////if ((rect2d.width > 4) && (rect2d.height > 4))
		//{
		//	tracker->update(srcImage, rect2d);
		//	rectangle(srcImage, rect2d, Scalar(255, 0, 123), 2, 1);
		//}
		//

		//��ʾSRCImage������
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
	// ������
	GetDlgItem(IDC_STATICPicture)->GetClientRect(&rect); //��ȡbox1�ͻ���
	IplImage *frame = NULL;
	IplImage frametemp;
	CvvImage cimg;
	CRect rect2 = rect;
	int w = desImage.cols;
	int h = desImage.rows;
	float s = 0;//���ű������˴�����
	ResizeWindow(rect2, w, h, s);//ʹ��Ƶ���ʺϿؼ���С����ʽ��ʾ

	frametemp = desImage;
	frame = cvCloneImage(&frametemp);
	cimg.CopyOf(frame, frame->nChannels);

	cimg.DrawToHDC(hdc, &rect2);//��ʾ���ؼ�
								//float ratio = cvGetCaptureProperty(capture, CV_CAP_PROP_POS_AVI_RATIO);//��ȡ��֡����Ƶ�е����λ��



}

void CMFCApplication1Dlg::ClearControl()
{
	//CMFCApplication1Dlg *pMMD = (CMFCApplication1Dlg*)AfxGetMainWnd();     //��ȡ������ָ��
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
	//�����V_Data
	if (!V_Data.empty())
	{
		V_Data.clear();
	}
	Img_Data temp_data;
	CString FilePathName;
	CFileDialog filedlg(TRUE, NULL, NULL, 0, _T("TXT (*.txt)|*.txt||"), NULL);
	filedlg.m_ofn.lpstrTitle = _T("��");
	if (filedlg.DoModal() == IDOK)
	{
		FilePathName = filedlg.GetPathName();

		fstream fin(FilePathName);
		if (!fin.is_open())
			AfxMessageBox(_T("��ȡʧ��"));
		int f_num = 0;
		fin >> f_num;//��ȡ�ļ�����
		fin >> b_num;//��ȡ���ο���
		string startflag, startstr;
		fin >> startflag;
		startstr = "/*****************start*************************/";
		//if (fin>>startflag == startstr);
		for (int i = 0; i<f_num; i++)
		{
			//��ȡͼƬ���
			fin >> temp_data.Img_code;

			//��ȡ�����㼯

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

			//cout << "�����㣺" << temp_data.ol_keypoints.size() << endl;
			//��ȡ������
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

			//��ȡ�ǵ�	
			Point cortmp;
			Box boxtmp;
			int whileint = 0;

			//for(int i=0;i<b_num;i++)

			//6����һ�顣��һ����Ϊboxnum����Ϊboltnum����Ϊstart_pt.x����Ϊstart_pt.y
			//��Ϊend_pt.x����Ϊend_pt.y
			int numIn6 = 0;
			while (!fin.eof())
			{
				fin >> startflag;
				if (startflag == startstr)
					break;

				numIn6 = whileint % 6;
				switch (numIn6)
				{
					//atoi��stringתint��atof��stringתfloat
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
			//ÿ�ζ������һ���ǵ�����飬���Զ���֮���ڴ˴�������Ǹ��ظ��ĵ�ɾ��
			//temp_data.ol_corners.pop_back();
			//cout << "�ǵ㣺" << temp_data.ol_corners.size() << endl;

			V_Data.push_back(temp_data);
			//�������գ��ǵ���ۻ�����һ��V_Data�ĳ�Ա��ȥ
			temp_data.ol_corners.clear();
			temp_data.ol_keypoints.clear();


			//AfxMessageBox(_T("here"));


		}
		fin.clear();
		fin.close();
		//AfxMessageBox(_T("��ȡ���"));
		/*int is1 = V_Data[0].ol_corners.size();
		CString stris1;
		stris1.Format(_T("%d"), is1);
		AfxMessageBox(stris1);*/
	}
}

Mat CMFCApplication1Dlg::get_H(vector<Img_Data> v_data, vector<Box> &cur_ol_corner)
{
	//�õ���������
	vector<KeyPoint> keypoints_scene;
	Mat descriptors_scene;
	get_online_data(keypoints_scene, descriptors_scene);
	//ÿһ����ƥ�䣬����ƥ�����
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
		//cout << "ģ�����:" << (*it).Img_code << "ƥ������" << cur_num << endl;
		if (cur_num > match_num)
		{
			//cout << "cur_matches:" << cur_matches.size() << endl;
			best_match = (*it);
			obj = cur_obj;
			scene = cur_scene;

		}

	}
	//cout << "���ƥ���ģ����룺" << best_match.Img_code << endl;
	cur_ol_corner = best_match.ol_corners;
	vector<unsigned char> listpoints;
	H = findHomography(obj, scene, CV_RANSAC, 3, listpoints);//����͸�ӱ任 
	return H;
}

void CMFCApplication1Dlg::get_online_data(vector<KeyPoint> &keypoints_scene, Mat &descriptors_scene)
{
	//��2��ʹ��SURF���Ӽ��ؼ���  
	//cv::SurfFeatureDetector surf(300);
	Ptr<Feature2D> surf = SURF::create(300);
	//vector<KeyPoint> keypoints_scene;//vectorģ���࣬����������͵Ķ�̬����  

	//��3������detect��������SURF�����ؼ��㣬������vector������  
	surf->detect(srcImage, keypoints_scene);

	//��4������������������������ 
	//cv::SurfDescriptorExtractor surfDesc;
	//Mat descriptors_scene;
	surf->compute(srcImage, keypoints_scene, descriptors_scene);
}

int CMFCApplication1Dlg::get_match_num(Mat des_template, Mat des_query, Img_Data best_match, vector<KeyPoint> keypoints_scene,
	vector<Point2f> &obj, vector<Point2f> &scene)
{
	//ʹ��FLANNƥ���㷨�е�knnƥ��������ƥ�� 
	FlannBasedMatcher matcher;
	vector<vector<DMatch>> matches;
	matcher.knnMatch(des_template, des_query, matches, 2);

	//���������ֲ���������ƥ��ɹ���ƥ����л�ȡ�ؼ���
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
	//����
	//Ptr<Tracker> tracker = TrackerMIL::create();
	//Rect2d dbbox(pre_corner[0].x, pre_corner[0].y, pre_corner[2].x,pre_corner[2].y);
	////imshow("��Ŀ��׷�١�", srcImage);
	//tracker->init(srcImage, dbbox);
	//rectangle(srcImage, dbbox, Scalar(255, 0, 123), 2, 1);
	//��Ŀ��ͼ���ϱ��Ŀ�������λ��
	line(srcImage, pre_corner[0], pre_corner[1], Scalar(255, 0, 123), 4);
	line(srcImage, pre_corner[1], pre_corner[2], Scalar(255, 0, 123), 4);
	line(srcImage, pre_corner[2], pre_corner[3], Scalar(255, 0, 123), 4);
	line(srcImage, pre_corner[3], pre_corner[0], Scalar(255, 0, 123), 4);

	//��ʾ���ս��
	//imshow("��Ŀ��׷�١�", srcImage);
	//waitKey(20);
}

void CMFCApplication1Dlg::getbox(vector<Img_Data> v_data, int box_i, vector<Point2f> &cur_corners, vector<Box> cur_ol_corner)
{
	vector<Box> ol_corners = cur_ol_corner;

	//ȡ�����Ϊbox_i�Ŀ򣬷���boxs
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
	//���û��Ҫ�ҵ�Ŀ�꣬��boxs�Ĵ�С��Ϊ0
	if (boxs.size() == 0)
	{
		boxs.push_back(start_pt);
		boxs.push_back(end_pt);
	}
	/*float is1 = box_i;
	CString stris1;
	stris1.Format(_T("%f"), is1);
	AfxMessageBox(stris1);*/
	////ȡ��ÿ����
	//vector<vector<Point2f>> boxs;
	//vector<Point2f> boxtmp(2);
	//for (int i = 0; i < ol_corners.size(); i++)
	//{
	//	boxtmp[i % 2] = ol_corners[i];
	//	if (i % 2 == 1)
	//		boxs.push_back(boxtmp);

	//}
	//�Ӵ���ͼƬ�л�ȡ�ǵ�  
	vector<Point2f> trans_corners(2);
	//trans_corners = boxs[box_i];
	//����͸�ӱ任  
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

	//������������ֵ�͸���
	int area = abs((scene_corners[0].x - scene_corners[2].x)*(scene_corners[0].y - scene_corners[2].y));
	//cout << "-------" << area << endl;
	/*if (area > 3000 && area<50000)
	{
	cur_corners = scene_corners;
	}*/
	//����
	cur_corners = scene_corners;

	double rect_x = cur_corners[1].x;
	double rect_y = cur_corners[1].y;
	double rect_w = abs(cur_corners[2].x - cur_corners[0].x);
	double rect_h = abs(cur_corners[2].y - cur_corners[0].y);
	Rect2d rect2d_update(rect_x, rect_y, rect_w, rect_h);
	//��boxs��СΪ0ʱ��rect2d�Ĵ�СҲΪ0
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
		edit->EnableWindow(TRUE); //��ʾ�ÿؼ�

	else
	{
		edit->EnableWindow(SW_HIDE);//���ظÿؼ�

	}
}

void CMFCApplication1Dlg::ShowRotationCtrl(bool srcflag)
{
	CEdit *edit1 = (CEdit*)GetDlgItem(IDC_BUTTON1);
	CEdit *edit2 = (CEdit*)GetDlgItem(IDC_BUTTON2);
	if (srcflag)
	{
		edit1->EnableWindow(TRUE); //��ʾ�ÿؼ�
		edit2->EnableWindow(TRUE); //��ʾ�ÿؼ�
		showbnicon();
	}
	else
	{
		rotationflag = 0;
		edit1->EnableWindow(SW_HIDE);//���ظÿؼ�
		edit2->EnableWindow(SW_HIDE);//���ظÿؼ�
		disablebnicon();
	}


}

void CMFCApplication1Dlg::OnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//AfxMessageBox(_T("������"));
	rotationflag++;
	rotationflag = rotationflag % 4;

	//rotation(srcImage, desImage);

	ClearControl();
	ShowRotationCtrl(true);
	ShowToControl(srcImage);
}

void CMFCApplication1Dlg::OnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	cimg1.DrawToHDC(hdc1, &rect1);//��ʾ���ؼ�

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
	cimg2.DrawToHDC(hdc2, &rect2);//��ʾ���ؼ�
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
	cimg1.DrawToHDC(hdc1, &rect1);//��ʾ���ؼ�

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
	cimg2.DrawToHDC(hdc2, &rect2);//��ʾ���ؼ�
}


void CMFCApplication1Dlg::OnMuTemplates()
{
	// TODO: �ڴ���������������
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
	pControlDC->SelectStockObject(BLACK_BRUSH); //���û�ˢ
	dlg->templatesets.GetPDC(pControlDC);
	//dlg->templatesets.ReadData(dlg->FilePathName, dlg->V_Aerasize);
	dlg->templatesets.GetAerasize(cap, dlg->V_Aerasize);

	CString str;
	pControlDC->MoveTo(50, 600);//���ߵĿ�ʼλ��
	pControlDC->LineTo(800, 600);
	pControlDC->MoveTo(50, 600);//���ߵĿ�ʼλ��
	pControlDC->LineTo(50, 50);
	pControlDC->MoveTo(40, 60);//���ߵĿ�ʼλ��
	pControlDC->LineTo(50, 50);
	pControlDC->LineTo(60, 60);
	pControlDC->MoveTo(790, 590);//���ߵĿ�ʼλ��
	pControlDC->LineTo(800, 600);
	pControlDC->LineTo(790, 610);
	pControlDC->TextOut(800, 600, _T("x-֡��"));
	pControlDC->TextOut(20, 30, _T("y-Ӱ������"));
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
	//pControlDC->SelectObject(&pen); //���û�ˢ
	pControlDC->MoveTo(50, 600 - dlg->V_Aerasize[0] * 4);
	for (int i = 1; i < aerasize; i++)
	{
		pControlDC->LineTo(50 + i * jiange, 600 - dlg->V_Aerasize[i] * 4);
	}
	return (void)NULL;
}