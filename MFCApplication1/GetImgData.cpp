#include "stdafx.h"
#include "GetImgData.h"

//����һ���ļ�
//const char* path = "E:\\VSFile\\0717-7.txt";

struct ImgData {
	vector<KeyPoint> keypoints;
	Mat descriptors;
	vector<vector<Point>> corners;
}offline;

//���캯����ʼ��ģ��ͼƬimg
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

//���ļ���׼��д����Ϣ
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

	//����detect��������SURF�����ؼ��㣬������vector������ 
	surf->detect(input, Img_keypoints);
	offline.keypoints = Img_keypoints;

	//���������������������� 
	//cv::SurfDescriptorExtractor surfDesc;
	Mat Img_descriptors;
	surf->compute(input, Img_keypoints, Img_descriptors);
	//surf->detectAndCompute(input,Mat(), Img_keypoints, Img_descriptors);
	offline.descriptors = Img_descriptors;

	//���ͼƬ���
	//fout << "0717-7" << endl;
	//str_temp = "0717-7";
	gid_str.clear();
	gid_str += CT2A(imgName);
	gid_str += " ";
	//��keypoints�����txt�ļ���
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
		

	//��descriptors�����txt�ļ���	
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