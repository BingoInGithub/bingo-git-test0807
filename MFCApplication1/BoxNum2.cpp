// BoxNum2.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "BoxNum2.h"
#include "afxdialogex.h"


// BoxNum2 对话框

IMPLEMENT_DYNAMIC(BoxNum2, CDialogEx)

BoxNum2::BoxNum2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

BoxNum2::~BoxNum2()
{
}

void BoxNum2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	GetDlgItem(IDC_bn2_EDIT1)->GetWindowText(bn_boxnum);
	//下面的命令是手动添加的，二者实现同样的功能
	GetDlgItem(IDC_bn2_EDIT2)->GetWindowText(bn_boltnum);
}


BEGIN_MESSAGE_MAP(BoxNum2, CDialogEx)
END_MESSAGE_MAP()


// BoxNum2 消息处理程序
int BoxNum2::getbnboxnum()
{
	//函数_ttoi(cstr)将CSting类的变量转为int类
	return _ttoi(bn_boxnum);
}

string BoxNum2::getbnboltnum()
{
	//函数CT2A将CString转为string
	//string str(W2A(bn_boltnum));
	return CT2A(bn_boltnum);
}

void BoxNum2::setboxnum(int boxn)
{
	boxnum = boxn;
}

void BoxNum2::setboltnum(std::string boltn)
{
	boltnum = boltn;
}

void BoxNum2::setstartpt_x(int pt_x)
{
	start_pt.x = pt_x;

}

void BoxNum2::setstartpt_y(int pt_y)
{

	start_pt.y = pt_y;
}

void BoxNum2::setendpt_x(int pt_x)
{
	end_pt.x = pt_x;

}

void BoxNum2::setendpt_y(int pt_y)
{

	end_pt.y = pt_y;
}

Point BoxNum2::getstartpt()
{
	return start_pt;
}

Point BoxNum2::getendpt()
{
	return end_pt;
}