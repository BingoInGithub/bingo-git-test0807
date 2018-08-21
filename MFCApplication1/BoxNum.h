#pragma once
#include <string>
#include <opencv2/core/core.hpp>  

using namespace std;
using namespace cv;
// BoxNum 对话框

class BoxNum : public CDialogEx
{
	DECLARE_DYNAMIC(BoxNum)

public:
	BoxNum(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~BoxNum();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


private:
	CString bn_boxnum;
	CString bn_boltnum;

	
public:
	int getbnboxnum();
	string getbnboltnum();
	
};

