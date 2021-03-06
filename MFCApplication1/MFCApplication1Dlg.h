
// MFCApplication1Dlg.h: 头文件
//

#pragma once
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <cstdio>  
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/tracking.hpp>
//#include <opencv2/nonfree/nonfree.hpp>
//#include <opencv2/nonfree/features2d.hpp>
#include <fstream>
#include <vector>

#include "GetImgData.h"
#include "Resource.h"
#include "Box.h"
#include "TemplateSets.h"


using namespace cv;
using namespace std;
using namespace xfeatures2d;

//储存离线数据的结构体
struct Img_Data {
	char Img_code[10];//图片编号
	int kp_num;//特征点数
	vector<KeyPoint> ol_keypoints;//特征点集
	int r, c;//描述符的行数和列数
	Mat ol_descriptors;//特征描述符
	vector<Box> ol_corners;//角点
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
	CImage image;//从本地读取的图片
	Mat org;//由image转换得到的Mat类图片，供后续的特征点提取等使用
	CPoint pt;
	CPoint start_pt;//矩形框的开始点
	CPoint end_pt;//矩形框的结束点，为start_pt的对角点
	
	bool m_startCorner;//角点绘制开关，点击生成按钮使其变为true，可以绘制矩形框
	bool m_startRect;//左键按下标志，当打开m_startCorner并按下左键时，该标志置为true，左键弹起置为false，完成一个矩形框的绘制
	GetImgData imgdata;//提取图像特征点、描述符、角点等信息的类GetImgData的对象
	string str;//储存特征点等信息的字符串，用于临时保存信息，当点击保存按钮时，其中的信息全部存入TXT文件

	CMenu menu;
	int targetNum;//待识别目标的编号，通过弹窗的下拉控件手动选择
	vector<Img_Data> V_Data;//存储读取到的图片信息，供匹配使用。其中的每一个元素都对应一张模板图片
	//VideoCapture cap;//用于匹配的视频
	//Mat srcImage;//cap输出的帧图像，用于匹配
	Mat H;//单应矩阵

	//当选择了一张图片并生成特征点时，文件数加一
	bool chooseflag;//选择图片标志
	bool generateflag;//生成标志
	int file_num;//文件块数量，每幅模板图片生成一个文件块，所有文件块存入一个TXT文件
	int box_num;//矩形框的最大编号

	HANDLE threadhandle;//子线程句柄
	CString FoldName;//文件夹名
	vector<CString> filePath;//文件路径
	vector<CString> fileName;//文件名
	string FilePathName;

	Mat srcImage;//cap输出的帧图像，用于匹配
	Mat desImage;//旋转图像的输出图像
	Ptr<Tracker> tracker;//跟踪器
	Rect2d rect2d;//跟踪方框
	bool updateflag;//控制是否更新rect2d的开关
	vector<Box> cur_ol_corner;
	vector<Point2f> pre_corner;
	int rotationflag;//旋转角度计数器
	TemplateSets templatesets;
	vector<int> V_Aerasize;
	bool resetflag;//重置开关
	bool updaterect2dflag;//更新rect的线程运行开关，关闭之后线程退出
public:
	//通过鼠标的操作绘制矩形框，逻辑为：
	//点击菜单栏的按钮打开绘制开关，当相应的绘制开关打开时，鼠标响应函数中的对应代码执行
	//当进行其他操作时，触发操作使相应开关关闭
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	


	void DrawRect(CPoint ptUL, CPoint ptLR);//画矩形框
	void DrawEllipse(CPoint point, int r);//画点

	//使显示的图像按控件窗口大小缩放并居中，其中rect是与控件大小相同的CRect对象
	//中间两个参数是图像的列数和行数
	//scale是最终图像与原始图像的缩放比例，其值为changed_cols/original_cols
	void ResizeWindow(CRect &rect, int img_cols, int img_rows, float &scale);

	//读取本地TXT文件中的信息,其中b_num为每幅模板图像上矩形框的数量
	//所有模板图像上标记的矩形框框选的目标、顺序、数量必须相同
	void ReadImgData(int &b_num);
	
	//所有带mu的都是菜单项
	afx_msg void OnMuChoosefile();
	afx_msg void OnMuGenerate();
	afx_msg void OnMuSave();
	afx_msg void OnMuVideo();
	afx_msg void OnMuMatch();
	afx_msg void OnButtonSubmit();
	//计算得到单应矩阵H
	Mat get_H(vector<Img_Data> v_data, vector<Box> &cur_ol_corner, Mat cur_img);
	//计算查询图像（视频帧图像）的特征点与描述符
	void get_online_data(Mat cur_img, vector<KeyPoint> &keypoints_scene, Mat &descriptors_scene);
	//计算最佳匹配点集的点数，用于选择最佳的模板
	int get_match_num(Mat des_template, Mat des_query, Img_Data best_match, vector<KeyPoint> keypoints_scene,
		              vector<Point2f> &obj, vector<Point2f> &scene);
	//使用仿射变换得到对应的矩形框，但是并不绘制
	void getbox(vector<Img_Data> v_data, int box_i, vector<Point2f> &cur_corners, vector<Box> cur_ol_corner);
	//框出找到的目标
	void drawbox(vector<Point2f> pre_corner);
	
	//将图片显示到控件上
	void ShowToControl(Mat stc_img);
	//清空控件
	void ClearControl();
	//将控件坐标系转为图片坐标系，包括了坐标的平移和缩放
	void ControlToImg(CPoint &point,Mat cti_img);

	//在类的成员函数中使用CreateThread创建子线程，子线程函数必须声明为类的静态成员函数
	//同时，由于静态成员函数中使用类的其他成员函数必须有具体的对象，因此在创建子线程时
	//要将this指针作为参数传入。当有多个参数时，使用结构体传参。
	//注意，结构体对象应该声明为全局变量，否则有可能出现结构体对象在传入子线程之前就被释放的风险
	static void tag(LPVOID args);
	
	//弹出对话框显示int型变量
	void AfxMB_i(int i);
	//控制“提交”按钮是否可用
	void ShowSubmitCtrl(bool sscflag);
	//追踪框更新子线程
	static void updaterect2d(LPVOID args);
	//点击逆时针旋转
	afx_msg void OnClickedButton1();
	//点击顺时针旋转
	afx_msg void OnClickedButton2();
	//右键按下，暂未添加响应事件
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//使两个旋转控件变亮
	void showbnicon();
	//使两个旋转控件变灰
	void disablebnicon();
	//旋转图像
	void rotation(Mat srcImg, Mat &desImg);
	//控制两个旋转控件变亮+启用/变灰+不启用
	void ShowRotationCtrl(bool srcflag);
	afx_msg void OnMuTemplates();

	static void GetAerasize(LPVOID args);
	void drawrect(CPoint a, CPoint b);
	afx_msg void OnMuReset();
	static void tracktarget(LPVOID args);
};