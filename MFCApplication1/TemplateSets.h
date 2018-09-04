#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <vector>
using namespace cv;
using namespace std;
using namespace xfeatures2d;

struct MarkedMat {
	bool sign;//所有图像的初始标记都为true
	Mat image;
	int index;
	//vector<int> area;
	vector<KeyPoint> keypoints;
	Mat descriptors;
	int aera;
};

struct OneToOne {
	int index;
	int bestmatchnum;
	int matchnum;
	double ratio;//最佳匹配数/min{模板特征点数，查询图像特征点数}
};

struct OneToMore {
	int index;
	vector<OneToOne> oto;
};

class TemplateSets
{
public:
	TemplateSets();
	~TemplateSets();
private:
	vector<MarkedMat> V_video;
	vector<OneToMore> V_otm;
	vector<int> V_aerasize;
	CDC *pDC;
	string str;
	CString cstring;
	int cap_count;
public:
	void ReadVideo(VideoCapture cap);
	void GetMatchNum(MarkedMat mmat1, MarkedMat mmat2, int &matchnum, int &bestmatchnum);
	void GetKPsandDescriptors(MarkedMat &mmat);
	void GetOTM();
	void GetAera();
	void GetAerasize(VideoCapture cap, vector<int> &aerasize);
	void GetPDC(CDC *pdc);
	void ReadData(string filepath, vector<int> &aerasize);
	void GetKeyFrames(vector<int> &framenum);
	void InitVvideo(string filepath);
	void SaveImage(vector<int> framenum);
};

