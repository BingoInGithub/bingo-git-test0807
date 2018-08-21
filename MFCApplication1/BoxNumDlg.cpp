// BoxNum.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "BoxNumDlg.h"
#include "afxdialogex.h"


// BoxNum 对话框

IMPLEMENT_DYNAMIC(BoxNumDlg, CDialogEx)

BoxNumDlg::BoxNumDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	//, bn_boxnum(_T(""))
{

	 // bn_boxnum = 0;
	//  ddddd = 0;
}

BoxNumDlg::~BoxNumDlg()
{
}

void BoxNumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text是通过类向导自动生成的
	//DDX_Text(pDX, IDC_BN_EDIT1, bn_boxnum);
	GetDlgItem(IDC_BN_EDIT1)->GetWindowText(bn_boxnum);
	//下面的命令是手动添加的，二者实现同样的功能
	GetDlgItem(IDC_BN_EDIT2)->GetWindowText(bn_boltnum);
	
}


BEGIN_MESSAGE_MAP(BoxNumDlg, CDialogEx)
END_MESSAGE_MAP()


// BoxNum 消息处理程序
int BoxNumDlg::getbnboxnum()
{
	//函数_ttoi(cstr)将CSting类的变量转为int类
	return _ttoi(bn_boxnum);
}

string BoxNumDlg::getbnboltnum()
{
	//函数CT2A将CString转为string
	//string str(W2A(bn_boltnum));
	return CT2A(bn_boltnum);
}

//void BoxNum::setboxnum(int boxn)
//{
//	boxnum = boxn;
//}
//
//void BoxNum::setboltnum(std::string boltn)
//{
//	boltnum = boltn;
//}
//
//void BoxNum::setstartpt_x(int pt_x)
//{
//	start_pt.x = pt_x;
//	
//}
//
//void BoxNum::setstartpt_y(int pt_y)
//{
//	
//	start_pt.y = pt_y;
//}
//
//void BoxNum::setendpt_x(int pt_x)
//{
//	end_pt.x = pt_x;
//	
//}
//
//void BoxNum::setendpt_y(int pt_y)
//{
//	
//	end_pt.y = pt_y;
//}
//
//Point BoxNum::getstartpt()
//{
//	return start_pt;
//}
//
//Point BoxNum::getendpt()
//{
//	return end_pt;
//}