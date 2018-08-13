
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
#include <vector>

#include "GetImgData.h"
#include "Resource.h"

using namespace cv;
using namespace std;

//储存离线数据的结构体，每个对象对应一个TXT
struct Img_Data {
	char Img_code[10];//图片编号
	int kp_num;//特征点数
	vector<KeyPoint> ol_keypoints;//特征点集
	int r, c;//描述符的行数和列数
	Mat ol_descriptors;//特征描述符
	vector<Point2f> ol_corners;//角点
};
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
	string str;//储存特征点等信息的字符串

	CMenu menu;
	int targetNum;//待识别目标的编号，手动输入
	vector<Img_Data> V_Data;//存储读取到的图片信息，供匹配使用
	VideoCapture cap;//用于匹配的视频
	Mat srcImage;
	Mat H;
	

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnBnClickedButtonSave();
//	afx_msg void OnBnClickedButtongenerate();

	void DrawRect(CPoint ptUL, CPoint ptLR);//画矩形框
	void DrawEllipse(CPoint point, int r);
	void ResizeWindow(CRect &rect, int img_cols, int img_rows, float &scale);//使显示的图像按控件窗口大小缩放并居中

	void ReadImgData();//读取本地信息
	afx_msg void OnMuChoosefile();
	afx_msg void OnMuGenerate();
	afx_msg void OnMuSave();
	afx_msg void OnMuMatch();
	afx_msg void OnMuVideo();

	Mat get_H(vector<Img_Data> v_data, vector<Point2f> &cur_ol_corner);
	void get_online_data(vector<KeyPoint> &keypoints_scene, Mat &descriptors_scene);
	int get_match_num(Mat des_template, Mat des_query, Img_Data best_match, vector<KeyPoint> keypoints_scene,
		              vector<Point2f> &obj, vector<Point2f> &scene);
	void drawbox(vector<Point2f> pre_corner);
	void getbox(vector<Img_Data> v_data, int box_i, vector<Point2f> &cur_corners, vector<Point2f> cur_ol_corner);

	void ShowToControl(Mat stc_img);
	void ClearControl();
	void ControlToImg(CPoint &point,Mat cti_img);//将控件坐标系转为图片坐标系
};


