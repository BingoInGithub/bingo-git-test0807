// BoxNum.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "BoxNum.h"
#include "afxdialogex.h"


// BoxNum 对话框

IMPLEMENT_DYNAMIC(BoxNum, CDialogEx)

BoxNum::BoxNum(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	//, bn_boxnum(_T(""))
{

	// bn_boxnum = 0;
	//  ddddd = 0;
}

BoxNum::~BoxNum()
{
}

void BoxNum::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text是通过类向导自动生成的
	//DDX_Text(pDX, IDC_BN_EDIT1, bn_boxnum);
	GetDlgItem(IDC_BN_EDIT1)->GetWindowText(bn_boxnum);
	//下面的命令是手动添加的，二者实现同样的功能
	GetDlgItem(IDC_BN_EDIT2)->GetWindowText(bn_boltnum);

}


BEGIN_MESSAGE_MAP(BoxNum, CDialogEx)
END_MESSAGE_MAP()


// BoxNum 消息处理程序
int BoxNum::getbnboxnum()
{
	//函数_ttoi(cstr)将CSting类的变量转为int类
	return _ttoi(bn_boxnum);
}

string BoxNum::getbnboltnum()
{
	//函数CT2A将CString转为string
	//string str(W2A(bn_boltnum));
	return CT2A(bn_boltnum);
}

