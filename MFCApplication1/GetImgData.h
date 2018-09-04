#pragma once
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <cstdio>  
#include <opencv2/opencv.hpp>
#include <iostream>
//#include <opencv2/nonfree/nonfree.hpp>
//#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <fstream>
using namespace cv;
using namespace std;
using namespace xfeatures2d;

class GetImgData
{
private:
	Mat org, img, tmp, H, srcImage2;//私有成员是待操作的模板图片
	//char gid_str[10000],str_temp[100];
	string gid_str;
public:
	const char* path;
	GetImgData();
	~GetImgData();
	//不使用构造函数对成员变量赋值，而是使用成员函数对成员变量赋值
	void Initorg(Mat input);
	//公有成员是对图片的一些操作方法
	void get_ImgData(Mat input, CString imgName);
	void on_mouse(int event, int x, int y, int flags, void *ustc);
	void openfile(const char* ipath);
	string get_str();
};

