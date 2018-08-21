#include "stdafx.h"
#include "Box.h"


Box::Box()
{
}


Box::~Box()
{
}

int Box::getboxnum()
{
	return boxnum;
}
void Box::setboxnum(int boxn)
{
	boxnum = boxn;
}

void Box::setboltnum(std::string boltn)
{
	boltnum = boltn;
}

void Box::setstartpt_x(int pt_x)
{
	start_pt.x = pt_x;
	
}

void Box::setstartpt_y(int pt_y)
{
	
	start_pt.y = pt_y;
}

void Box::setendpt_x(int pt_x)
{
	end_pt.x = pt_x;
	
}

void Box::setendpt_y(int pt_y)
{
	
	end_pt.y = pt_y;
}

Point2f Box::getstartpt()
{
	return start_pt;
}

Point2f Box::getendpt()
{
	return end_pt;
}