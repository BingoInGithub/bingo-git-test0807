// ChooseTarget.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "ChooseTarget.h"
#include "afxdialogex.h"


// ChooseTarget 对话框

IMPLEMENT_DYNAMIC(ChooseTarget, CDialogEx)

ChooseTarget::ChooseTarget(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

ChooseTarget::~ChooseTarget()
{
}

void ChooseTarget::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ChooseTarget, CDialogEx)
	ON_BN_CLICKED(IDOK, &ChooseTarget::OnBnClickedOk)
END_MESSAGE_MAP()


// ChooseTarget 消息处理程序

CString ChooseTarget::geteditstr()
{
	return editstr;
}
void ChooseTarget::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CEdit *edit;
	edit = (CEdit*)GetDlgItem(IDC_EDIT1);
	
	edit->GetWindowText(editstr);
	//editstr = "3";
	//AfxMessageBox(editstr);

	
	CDialogEx::OnOK();
}
