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
//ChooseTarget::ChooseTarget(int b_num, CWnd* pParent = nullptr) : CDialogEx(IDD_DIALOG1, pParent)
//{
//	box_num = b_num;
//}
ChooseTarget::~ChooseTarget()
{
}
BOOL ChooseTarget::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString target_num;
	if(box_num==0)
		m_cb.AddString(_T("0"));
	else
		for (int i = 0; i < box_num; i++)
		{
			target_num.Format(_T("%d"), i + 1);
			m_cb.AddString(target_num);
		}
	
	m_cb.SetCurSel(0);

	return TRUE;
}
void ChooseTarget::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cb);
}


BEGIN_MESSAGE_MAP(ChooseTarget, CDialogEx)
	ON_BN_CLICKED(IDOK, &ChooseTarget::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &ChooseTarget::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// ChooseTarget 消息处理程序
void ChooseTarget::getboxnum(int b_num)
{
	box_num = b_num;
}
CString ChooseTarget::geteditstr()
{
	return editstr;
}
void ChooseTarget::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CEdit *edit;
	edit = (CEdit*)GetDlgItem(IDC_COMBO1);
	
	edit->GetWindowText(editstr);
	//editstr = "3";
	//AfxMessageBox(editstr);

	
	CDialogEx::OnOK();
}


void ChooseTarget::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}
