#pragma once


// ChooseTarget 对话框

class ChooseTarget : public CDialogEx
{
	DECLARE_DYNAMIC(ChooseTarget)

public:
	ChooseTarget(CWnd* pParent = nullptr);   // 标准构造函数
	//ChooseTarget(int b_num, CWnd* pParent = nullptr);//自定义构造函数
	virtual ~ChooseTarget();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	BOOL OnInitDialog();
private:
	CString editstr;
	int box_num;
public:
	CString geteditstr();
	void getboxnum(int b_num);
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_cb;
};
