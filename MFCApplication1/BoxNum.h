#pragma once
#include <string>
#include <opencv2/core/core.hpp>  

using namespace std;
using namespace cv;
// BoxNum �Ի���

class BoxNum : public CDialogEx
{
	DECLARE_DYNAMIC(BoxNum)

public:
	BoxNum(CWnd* pParent = nullptr);   // ��׼���캯��
	virtual ~BoxNum();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


private:
	CString bn_boxnum;
	CString bn_boltnum;

	
public:
	int getbnboxnum();
	string getbnboltnum();
	
};

