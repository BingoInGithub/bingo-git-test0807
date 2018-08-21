#pragma once
#include <string>
#include <opencv2/core/core.hpp>  

using namespace std;
using namespace cv;
class Box
{
public:
	Box();
	~Box();
private:
	CString bn_boxnum;
	CString bn_boltnum;

	int boxnum;
	string boltnum;
	Point start_pt;
	Point end_pt;
public:
	int getboxnum();
	void setboxnum(int boxn);
	void setboltnum(string boltn);
	void setstartpt_x(int pt_x);
	void setstartpt_y(int pt_y);
	void setendpt_x(int pt_x);
	void setendpt_y(int pt_y);

	Point2f getstartpt();
	Point2f getendpt();
};

