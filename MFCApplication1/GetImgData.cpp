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


void GetImgData::get_ImgData(Mat input)
{
	cv::SurfFeatureDetector surf(300);
	vector<KeyPoint> Img_keypoints;

	//����detect��������SURF�����ؼ��㣬������vector������  
	surf.detect(input, Img_keypoints);
	offline.keypoints = Img_keypoints;

	//���������������������� 
	cv::SurfDescriptorExtractor surfDesc;
	Mat Img_descriptors;
	surfDesc.compute(input, Img_keypoints, Img_descriptors);
	offline.descriptors = Img_descriptors;

	//���ͼƬ���
	//fout << "0717-7" << endl;
	//str_temp = "0717-7";
	gid_str.clear();
	gid_str += "0717-7\n";
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
		//fout << (*ikp).pt.x << "\t" << (*ikp).pt.y << "\t" << (*ikp).size << "\t" << (*ikp).angle << "\t\t" << (*ikp).response << "\t\t" << (*ikp).octave << "\t" << (*ikp).class_id << endl;;

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

void GetImgData::on_mouse(int event, int x, int y, int flags, void *ustc)//event����¼����ţ�x,y������꣬flags��ק�ͼ��̲����Ĵ���  
{

	static Point pre_pt(-1, -1);//��ʼ����  
	static Point cur_pt(-1, -1);//ʵʱ����  
	char temp[16];
	if (event == CV_EVENT_LBUTTONDOWN)//������£���ȡ��ʼ���꣬����ͼ���ϸõ㴦��Բ  
	{
		org.copyTo(img);//��ԭʼͼƬ���Ƶ�img��  
						//sprintf_s(temp, "   (%d,%d)", x, y);
		pre_pt = Point(x, y);
		//putText(img, temp, pre_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);//�ڴ�������ʾ����  
		//circle(img, pre_pt, 15, Scalar(0, 225, 0, 0), CV_FILLED, CV_AA, 0);//��Բ  
		imshow("img", img);
	}
	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//���û�а��µ����������ƶ��Ĵ�����  
	{
		img.copyTo(tmp);//��img���Ƶ���ʱͼ��tmp�ϣ�������ʾʵʱ����  
						//sprintf_s(temp, "  (%d,%d)", x, y);
		cur_pt = Point(x, y);
		//putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));//ֻ��ʵʱ��ʾ����ƶ�������  
		imshow("img", tmp);
	}
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//�������ʱ������ƶ�������ͼ���ϻ�����  
	{
		img.copyTo(tmp);
		//sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		//putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		rectangle(tmp, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 4, 8, 0);//����ʱͼ����ʵʱ��ʾ����϶�ʱ�γɵľ���  
		imshow("img", tmp);
	}
	else if (event == CV_EVENT_LBUTTONUP)//����ɿ�������ͼ���ϻ�����  
	{
		org.copyTo(img);
		cur_pt = Point(x, y);
		//putText(img, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		//circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
		if (pre_pt != cur_pt)
		{
			rectangle(img, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//���ݳ�ʼ��ͽ����㣬�����λ���img��  
			imshow("img", img);
			img.copyTo(tmp);

			//����Ľǵ����ImgData
			vector<Point> tmp_corners(4);
			tmp_corners[0] = pre_pt;
			tmp_corners[1] = cvPoint(pre_pt.x, cur_pt.y);
			tmp_corners[2] = cur_pt;
			tmp_corners[3] = cvPoint(cur_pt.x, pre_pt.y);

			//���ǵ����txt�ļ�
			/*fout << tmp_corners[0].x << " " << tmp_corners[0].y << " " << tmp_corners[1].x << " " << tmp_corners[1].y
				<< " " << tmp_corners[2].x << " " << tmp_corners[2].y << " " << tmp_corners[3].x << " "
				<< tmp_corners[3].y << endl;*/

		}
		waitKey(0);
	}
}


string GetImgData::get_str()
{
	return gid_str;
}