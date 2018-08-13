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


void GetImgData::get_ImgData(Mat input)
{
	cv::SurfFeatureDetector surf(300);
	vector<KeyPoint> Img_keypoints;

	//调用detect函数检测出SURF特征关键点，保存在vector容器中  
	surf.detect(input, Img_keypoints);
	offline.keypoints = Img_keypoints;

	//计算描述符（特征向量） 
	cv::SurfDescriptorExtractor surfDesc;
	Mat Img_descriptors;
	surfDesc.compute(input, Img_keypoints, Img_descriptors);
	offline.descriptors = Img_descriptors;

	//输出图片编号
	//fout << "0717-7" << endl;
	//str_temp = "0717-7";
	gid_str.clear();
	gid_str += "0717-7\n";
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
		//fout << (*ikp).pt.x << "\t" << (*ikp).pt.y << "\t" << (*ikp).size << "\t" << (*ikp).angle << "\t\t" << (*ikp).response << "\t\t" << (*ikp).octave << "\t" << (*ikp).class_id << endl;;

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

void GetImgData::on_mouse(int event, int x, int y, int flags, void *ustc)//event鼠标事件代号，x,y鼠标坐标，flags拖拽和键盘操作的代号  
{

	static Point pre_pt(-1, -1);//初始坐标  
	static Point cur_pt(-1, -1);//实时坐标  
	char temp[16];
	if (event == CV_EVENT_LBUTTONDOWN)//左键按下，读取初始坐标，并在图像上该点处划圆  
	{
		org.copyTo(img);//将原始图片复制到img中  
						//sprintf_s(temp, "   (%d,%d)", x, y);
		pre_pt = Point(x, y);
		//putText(img, temp, pre_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);//在窗口上显示坐标  
		//circle(img, pre_pt, 15, Scalar(0, 225, 0, 0), CV_FILLED, CV_AA, 0);//划圆  
		imshow("img", img);
	}
	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数  
	{
		img.copyTo(tmp);//将img复制到临时图像tmp上，用于显示实时坐标  
						//sprintf_s(temp, "  (%d,%d)", x, y);
		cur_pt = Point(x, y);
		//putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));//只是实时显示鼠标移动的坐标  
		imshow("img", tmp);
	}
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上划矩形  
	{
		img.copyTo(tmp);
		//sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		//putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		rectangle(tmp, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 4, 8, 0);//在临时图像上实时显示鼠标拖动时形成的矩形  
		imshow("img", tmp);
	}
	else if (event == CV_EVENT_LBUTTONUP)//左键松开，将在图像上划矩形  
	{
		org.copyTo(img);
		cur_pt = Point(x, y);
		//putText(img, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		//circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
		if (pre_pt != cur_pt)
		{
			rectangle(img, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//根据初始点和结束点，将矩形画到img上  
			imshow("img", img);
			img.copyTo(tmp);

			//将框的角点存入ImgData
			vector<Point> tmp_corners(4);
			tmp_corners[0] = pre_pt;
			tmp_corners[1] = cvPoint(pre_pt.x, cur_pt.y);
			tmp_corners[2] = cur_pt;
			tmp_corners[3] = cvPoint(cur_pt.x, pre_pt.y);

			//将角点存入txt文件
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