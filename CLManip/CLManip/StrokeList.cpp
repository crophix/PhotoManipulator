#include "StrokeList.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

StrokeList::StrokeList(void)
{
	count = 0;
	strokes = new brushStroke[1];
}

StrokeList::~StrokeList(void)
{
}

void StrokeList::addStroke(int radius, int x, int y, Mat referenceImage)
{
	brushStroke* temp = new brushStroke[++count];
	int j = rand() % count;
	temp[j].r = radius;
	temp[j].color = referenceImage.at<Vec3b>(y,x);
	temp[j].cntr = Point(x,y);
	int o = 0;
	for (int i = 0; i < count-1; ++i)
	{
		if (j == i) o++;
		temp[i+o].r = strokes[i].r;
		temp[i+o].cntr = strokes[i].cntr;
		temp[i+o].color = strokes[i].color;
	}
	delete strokes;
	strokes = temp;
}

int StrokeList::getCount()
{
	return count;
}

void StrokeList::applyStrokes(Mat canvas)
{
	for (int i = 0; i < count; ++i)
	{
		circle(canvas, strokes[i].cntr, strokes[i].r, Scalar(strokes[i].color), -1, 8);
	}
	count = 0;
}