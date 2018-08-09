
// MFCApplication1Dlg.h: 头文件
//

#pragma once
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <cstdio>  
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <fstream>

#include "GetImgData.h"
#include "Resource.h"

using namespace cv;
using namespace std;


// CMFCApplication1Dlg 对话框
class CMFCApplication1Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnEnChangeEdit1();
//	afx_msg void OnBnClickedButtonChoosefile();

private:
	CImage image;
	Mat org;
	CPoint pt;
	CPoint start_pt;
	CPoint end_pt;
	bool m_startRect;
	bool m_startCorner;

	GetImgData imgdata;
	string str;

	CMenu menu;

	

	

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnBnClickedButtonSave();
//	afx_msg void OnBnClickedButtongenerate();

	void DrawRect(CPoint ptUL, CPoint ptLR);
	void DrawEllipse(CPoint point, int r);
	void ResizeWindow(CRect &rect, int width, int height);
	afx_msg void OnMuChoosefile();
	afx_msg void OnMuGenerate();
	afx_msg void OnMuSave();
	afx_msg void OnMuChoosetarget();
	afx_msg void OnMuVideo();
};


