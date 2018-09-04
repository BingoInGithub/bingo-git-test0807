#include "stdafx.h"
#include "TemplateSets.h"


TemplateSets::TemplateSets()
{
}

int col1 = 660, col2 = 780, col3 = 840;

TemplateSets::~TemplateSets()
{
}

void TemplateSets::ReadVideo(VideoCapture cap)
{
	pDC->TextOut(col1, 30, _T("get V_video.."));
	Mat rvimg;
	MarkedMat rvvideo;
	cap_count = cap.get(CV_CAP_PROP_FRAME_COUNT);//得到视频总帧数
	str += (to_string(cap_count) + " ");
	for (int i = 0; i < cap_count; i++)
	{
		cstring.Format(_T("%d/%d"), i+1, cap_count);
		pDC->TextOut(col2, 30, cstring);
		cap >> rvimg;
		rvvideo.image = rvimg;
		rvvideo.sign = true;
		rvvideo.index = i;
		GetKPsandDescriptors(rvvideo);
		V_video.push_back(rvvideo);
	}
	pDC->TextOut(col3, 30, _T("done"));
}

void TemplateSets::GetMatchNum(MarkedMat mmat1, MarkedMat mmat2, int &matchnum, int &bestmatchnum)
{
	Mat des_template = mmat1.descriptors;
	Mat des_query = mmat2.descriptors;
	//使用FLANN匹配算法中的knn匹配器进行匹配 
	FlannBasedMatcher matcher;
	vector<vector<DMatch>> matches;
	matcher.knnMatch(des_template, des_query, matches, 2);
	matchnum = matches.size();
	//定义两个局部变量，从匹配成功的匹配对中获取关键点
	DMatch bettermatch, goodmatch;
	vector<DMatch> bestmatch;
	double matchRatio = 0.0;
	for (unsigned int i = 0; i < matches.size(); i++)
	{
		bettermatch = matches[i][0];
		goodmatch = matches[i][1];
		matchRatio = (1.0*bettermatch.distance) / goodmatch.distance;
		if (matchRatio < 0.8)
		{
			bestmatch.push_back(bettermatch);
			//cout << "---"<<best_match.ol_keypoints.size() << "   " << bettermatch.queryIdx << endl;
			//obj.push_back(best_match.ol_keypoints[bettermatch.queryIdx].pt);
			//scene.push_back(keypoints_scene[bettermatch.trainIdx].pt);
		}

	}
	bestmatchnum = bestmatch.size();
}

void TemplateSets::GetKPsandDescriptors(MarkedMat &mmat)
{
	//使用SURF算子检测关键点  
	Ptr<Feature2D> surf = SURF::create(300);

	//调用detect函数检测出SURF特征关键点，保存在vector容器中  
	surf->detect(mmat.image, mmat.keypoints);

	//计算描述符（特征向量） 
	surf->compute(mmat.image, mmat.keypoints, mmat.descriptors);
}

void TemplateSets::GetOTM()
{
	pDC->TextOut(col1, 50, _T("get V_otm.."));

	OneToMore otm;
	OneToOne oto;
	int matchnum = 0;
	int bestmatchnum = 0;
	int minkpnum = 0;
	V_otm.clear();
	for (int i = 0; i < cap_count; i++)
	{
		cstring.Format(_T("%d/%d"), i + 1, cap_count);
		pDC->TextOut(col2, 50, cstring);

		otm.oto.clear();
		otm.index = i;
		str += (to_string(i)+" ");
		for (int j = 0; j < cap_count; j++)
		{
			oto.index = j;
			
			GetMatchNum(V_video[i], V_video[j], matchnum, bestmatchnum);
			oto.matchnum = matchnum;
			oto.bestmatchnum = bestmatchnum;
			minkpnum = min(V_video[i].keypoints.size(), V_video[j].keypoints.size());
			oto.ratio = (bestmatchnum*1.0) / minkpnum;
			otm.oto.push_back(oto);

			str += (to_string(j) + " ");
			str += (to_string(matchnum) + " ");
			str += (to_string(bestmatchnum) + " ");
			str += (to_string(oto.ratio) + " ");
		}
		V_otm.push_back(otm);
	}
	pDC->TextOut(col3, 50, _T("done"));
}

void TemplateSets::GetAera()
{
	pDC->TextOut(col1, 70, _T("get V_aerasize.."));
	V_aerasize.clear();
	int threshold = 1000;
	int aerasize = 0;
	for (int i = 0; i < cap_count; i++)
	{
		cstring.Format(_T("%d/%d"), i + 1, cap_count);
		pDC->TextOut(col2, 70, cstring);
		for (int j = 0; j < cap_count; j++)
		{
			if (V_otm[i].oto[j].bestmatchnum > 500)
			{
				aerasize++;
			}
		}
		V_aerasize.push_back(aerasize);
		V_video[i].aera = aerasize;
		aerasize = 0;
	}
	pDC->TextOut(col3, 70, _T("done"));
}

void TemplateSets::GetAerasize(VideoCapture cap, vector<int> &aerasize)
{
	if (!cap.isOpened())
	{
		AfxMessageBox(_T("请导入视频"));
	}
	else
	{
		CFileDialog filedlg(FALSE, _T("txt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("TXT (*.txt)|*.txt||"));
		filedlg.m_ofn.lpstrTitle = _T("打开");
		CString FilePathName;
		if (filedlg.DoModal() == IDOK)
		{
			FilePathName = filedlg.GetPathName();
		}
		str.clear();
		str = "";

		pDC->TextOut(col1, 10, _T("Begin.."));
		ReadVideo(cap);
		GetOTM();
		GetAera();
		aerasize = V_aerasize;

		pDC->TextOut(col1, 90, _T("Done!"));

		ofstream fout(FilePathName);
		//AfxMB_i(file_num);
			
		int str_size = str.size();
		for (int i = 0; i < str_size; i++)
		{
			fout << str[i];
			if (i == str_size - 1)
				pDC->TextOut(col1, 110, _T("已保存"));
		}
		fout.close();	
	}
	
}

void TemplateSets::GetPDC(CDC *pdc)
{
	pDC = pdc;
}

void TemplateSets::ReadData(string filepath, vector<int> &aerasize)
{
	CFileDialog filedlg(TRUE, _T("txt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("TXT (*.txt)|*.txt||"));
	filedlg.m_ofn.lpstrTitle = _T("打开");
	CString FilePathName;
	if (filedlg.DoModal() == IDOK)
	{
		FilePathName = filedlg.GetPathName();
		fstream fin(FilePathName);
		
		pDC->TextOut(col1, 30, _T("Begin.."));
		pDC->TextOut(col1, 50, _T("get V_otm.."));
		fin >> cap_count;
		OneToMore otm;
		OneToOne oto;
		int matchnum = 0;
		int bestmatchnum = 0;
		int minkpnum = 0;
		V_otm.clear();
		for (int i = 0; i < cap_count; i++)
		{
			cstring.Format(_T("%d/%d"), i + 1, cap_count);
			pDC->TextOut(col2, 50, cstring);

			otm.oto.clear();
			fin >> otm.index ;
			
			for (int j = 0; j < cap_count; j++)
			{
				fin >> oto.index ;
				fin >> oto.matchnum;
				fin >> oto.bestmatchnum;
				fin >> oto.ratio;
				otm.oto.push_back(oto);
			}
			V_otm.push_back(otm);
		}
		pDC->TextOut(col3, 50, _T("done"));

		InitVvideo(filepath);
		GetAera();
		aerasize = V_aerasize;
		vector<int> keyframenum;
		GetKeyFrames(keyframenum);
		int keyframenumsize = keyframenum.size();
		CString framecstr;
		framecstr.Format(_T("%d"), keyframenumsize);
		pDC->TextOut(20, 10, framecstr);
		/*for (int i = 0; i < keyframenumsize; i++)
		{
			framecstr.Format(_T("%d"), keyframenum[i]);
			pDC->TextOut(20 + 50 * i, 10, framecstr);
		}*/
			
		str.clear();
		while (!fin.eof())
		{
			fin >> str;
		}
		
		if(str.size()==0)
			pDC->TextOut(col1, 90, _T("Done!"));
		else
			pDC->TextOut(col1, 90, _T("Failed!"));
		fin.close();

		SaveImage(keyframenum);
	}
}

void TemplateSets::GetKeyFrames(vector<int> &framenum)
{
	int biggistindex = 0;
	MarkedMat gkfmat = V_video[0];
	
	for (int i = 0; i < cap_count; i++)
	{
		if (V_video[i].sign == true)
		{
			gkfmat = gkfmat.aera > V_video[i].aera ? gkfmat : V_video[i];
			//biggistindex = gkfmat.index;
		}
	}
	biggistindex = gkfmat.index;
	framenum.push_back(biggistindex);
	int totalframe = 0;
	int framenumsize = framenum.size();
	for (int i = 0; i < framenumsize; i++)
		totalframe += V_video[framenum[i]].aera;
	if (totalframe > cap_count)
		return;
	else
	{
		int changeindex = 0;
		vector<OneToOne> gkfoto = V_otm[biggistindex].oto;
		
		CString cistr;
		for (int i = 0; i < cap_count; i++)
		{
			if (gkfoto[i].bestmatchnum > 500)
			{
				changeindex = gkfoto[i].index;
				V_video[changeindex].sign = false;
			}
				
		}
			
		GetKeyFrames(framenum);
	}
}

void TemplateSets::InitVvideo(string filepath)
{
	V_video.clear();
	MarkedMat rvvideo;
	Mat IVimg;
	VideoCapture cap;
	cap.open(filepath);
	for (int i = 0; i < cap_count; i++)
	{
		rvvideo.sign = true;
		rvvideo.index = i;
		cap >> IVimg;
		Mat temp = IVimg.clone();
		rvvideo.image = temp;
		V_video.push_back(rvvideo);
		temp.release();
	}
	
	cap.~VideoCapture();
	CString strde;
	strde.Format(_T("%d"), V_video[65].index);
	pDC->TextOut(40, 40, strde);
	imshow("dfsf", V_video.at(400).image);
	waitKey(1);
}

void TemplateSets::SaveImage(vector<int> framenum)
{
		CString FoldPathName;
		//打开一个文件夹
		CFolderPickerDialog fd(NULL, 0, NULL, 0);
		if (fd.DoModal() == IDOK)
		{
			string des;
			FoldPathName = fd.GetPathName();
			int framenumsize = framenum.size();
			Mat saveimg;
			int j = V_video.size();
			for (int i = 0; i < framenumsize; i++)
			{
				des = FoldPathName+"\\template-" + to_string(i) + ".jpg";
				//saveimg = V_video[framenum[i]].image;
				imwrite(des, V_video[framenum[i]].image);
				//imwrite(des, V_video[i+50].image);
			}
		}
	
}