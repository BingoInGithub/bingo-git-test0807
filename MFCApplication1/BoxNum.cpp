// BoxNum.cpp: ʵ���ļ�
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "BoxNum.h"
#include "afxdialogex.h"


// BoxNum �Ի���

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
	//DDX_Text��ͨ�������Զ����ɵ�
	//DDX_Text(pDX, IDC_BN_EDIT1, bn_boxnum);
	GetDlgItem(IDC_BN_EDIT1)->GetWindowText(bn_boxnum);
	//������������ֶ���ӵģ�����ʵ��ͬ���Ĺ���
	GetDlgItem(IDC_BN_EDIT2)->GetWindowText(bn_boltnum);

}


BEGIN_MESSAGE_MAP(BoxNum, CDialogEx)
END_MESSAGE_MAP()


// BoxNum ��Ϣ�������
int BoxNum::getbnboxnum()
{
	//����_ttoi(cstr)��CSting��ı���תΪint��
	return _ttoi(bn_boxnum);
}

string BoxNum::getbnboltnum()
{
	//����CT2A��CStringתΪstring
	//string str(W2A(bn_boltnum));
	return CT2A(bn_boltnum);
}

