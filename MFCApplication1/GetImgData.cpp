#include "stdafx.h"
#include "GetImgData.h"

//创建一个文件
//const char* path = "E:\\VSFile\\0717-7.txt";

struct ImgData {
	vector<KeyPoint> keypoints;
	Mat descriptors;
	vector<vector<Point>> corners;
}offline;

//构造函数初始化模板图片img
GetImgData::GetImgData()
{
}


GetImgData::~GetImgData()
{
}

void GetImgData::Initorg(Mat input)
{
	org = input;
	
}

//打开文件，准备写入信息
char *path = "E:\\";
void GetImgData::openfile(const char* ipath)
{
	path = ipath;
	
}


void GetImgData::get_ImgData(Mat input, CString imgName)
{
	Ptr<Feature2D> surf = SURF::create(300);
	//SurfFeatureDetector surf(300);
	vector<KeyPoint> Img_keypoints;

	//调用detect函数检测出SURF特征关键点，保存在vector容器中 
	surf->detect(input, Img_keypoints);
	offline.keypoints = Img_keypoints;

	//计算描述符（特征向量） 
	//cv::SurfDescriptorExtractor surfDesc;
	Mat Img_descriptors;
	surf->compute(input, Img_keypoints, Img_descriptors);
	//surf->detectAndCompute(input,Mat(), Img_keypoints, Img_descriptors);
	offline.descriptors = Img_descriptors;

	//输出图片编号
	//fout << "0717-7" << endl;
	//str_temp = "0717-7";
	gid_str.clear();
	gid_str += CT2A(imgName);
	gid_str += " ";
	//将keypoints输出到txt文件中
	//fout << offline.keypoints.size() << endl;
	gid_str += to_string(offline.keypoints.size());
	gid_str += " \n";
	vector<KeyPoint>::iterator ikp;
	for (ikp = offline.keypoints.begin(); ikp != offline.keypoints.end(); ikp++)
	{
		gid_str += (to_string((*ikp).pt.x) + " ");
		gid_str += (to_string((*ikp).pt.y) + " ");
		gid_str += (to_string((*ikp).size) + " ");
		gid_str += (to_string((*ikp).angle) + " ");
		gid_str += (to_string((*ikp).response) + " ");
		gid_str += (to_string((*ikp).octave) + " ");
		gid_str += (to_string((*ikp).class_id) + " \n");
	}
		

	//将descriptors输出到txt文件中	
	gid_str += (to_string(offline.descriptors.rows) + " ");
	gid_str += (to_string(offline.descriptors.cols) + " \n");
	//fout << offline.descriptors.rows << " " << offline.descriptors.cols << endl;
	for (int i = 0; i < offline.descriptors.rows; i++)
		for (int j = 0; j < offline.descriptors.cols; j++)
			gid_str += (to_string(offline.descriptors.at<float>(i, j)) + " ");
			//fout << offline.descriptors.at<float>(i, j) << " ";
	
	gid_str += " \n";
}

string GetImgData::get_str()
{
	return gid_str;
}