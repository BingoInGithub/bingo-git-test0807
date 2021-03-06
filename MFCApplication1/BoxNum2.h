#pragma once
#include <string>
#include <opencv2/core/core.hpp>  

using namespace std;
using namespace cv;

// BoxNum2 对话框

class BoxNum2 : public CDialogEx
{
	DECLARE_DYNAMIC(BoxNum2)

public:
	BoxNum2(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~BoxNum2();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString bn_boxnum;
	CString bn_boltnum;

	int boxnum;
	string boltnum;
	Point start_pt;
	Point end_pt;
public:
	int getbnboxnum();
	string getbnboltnum();
	void setboxnum(int boxn);
	void setboltnum(string boltn);
	void setstartpt_x(int pt_x);
	void setstartpt_y(int pt_y);
	void setendpt_x(int pt_x);
	void setendpt_y(int pt_y);

	Point getstartpt();
	Point getendpt();
};
