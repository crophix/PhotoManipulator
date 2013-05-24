#include "StrokeList.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

using namespace cv;
using namespace std;

const double PI = atan(1.0)*4;

StrokeList::StrokeList(Mat referenceImage, Mat canvas, int brushSize)
{
	maxStrokes = ((referenceImage.cols / brushSize)+1)* ((referenceImage.rows / brushSize)+1);
	strokes = new curvedStroke[maxStrokes];
	isFull = new bool[maxStrokes];
	for (int i = 0; i < maxStrokes; ++i)
		isFull[i] = false;
	Mat greyImg;
	cvtColor(referenceImage, greyImg, CV_BGR2GRAY);
	Sobel(greyImg, xImg, CV_16S, 1, 0);
	Sobel(greyImg, yImg, CV_16S, 0, 1);
	absdiff(referenceImage, canvas, diff);
}

StrokeList::~StrokeList(void)
{
}

void StrokeList::addCurvedStroke(int radius, int x, int y, Mat referenceImage, Mat canvas)
{
	// select a random point in the sequence of strokes and put the new stroke there
	int j = rand() % maxStrokes;
	while (isFull[j])
		j = rand() % maxStrokes;
	strokes[j].r = radius;
	strokes[j].color = referenceImage.at<Vec3b>(y,x);
	strokes[j].cntrList[0] = Point(x,y); // first point in the curve
	for (int i = 1; i < MAXSTROKELEN; ++i)
		strokes[j].cntrList[i] = Point(-1,-1);
	isFull[j] = true;
	
	// Setup the starting point at the given coordinate
	Point	cur		= Point(x,y);
	double	lastx	= 0.0;
	double	lasty	= 0.0;
	Mat d2;
	absdiff(referenceImage, Scalar(strokes[j].color), d2);
	for (int i = 1; i < MAXSTROKELEN; ++i)
	{
		if (i >= MINSTROKELEN)
		{
			// Compute the color difference between the reference image and the canvas and between the reference
			//		image and the chosen color.  The efficiency of this can be improved.			
			if (diff.at<uchar>(cur) < d2.at<uchar>(cur)) 
			{
				break;
			}
		}
		double dx = xImg.at<short>(cur);
		double dy = yImg.at<short>(cur);
		// Caculate the direction of gradient change
		double dir = atan2(dy,dx);
		// compute a normal direction
		double deltax = -sin(dir);
		double deltay = cos(dir);
		if (lastx*deltax + lasty*deltay < 0) // reverse direction if needed
		{
			deltax = -deltax;
			deltay = -deltay;
		}
		int tempx = max(0, min((int)(deltax*radius+cur.x), canvas.cols-1));
		int tempy = max(0, min((int)(deltay*radius+cur.y), canvas.rows-1));
		cur = Point(tempx, tempy);
		lastx = deltax;
		lasty = deltay;
		strokes[j].cntrList[i] = cur;
	}
}

void StrokeList::applyCurvedStrokes(Mat canvas)
{
	// For each stroke in our list connect the points with a line of the correct width
	for (int i = 0; i < maxStrokes; ++i)
	{
		if (isFull[i])
		{
			if (strokes[i].cntrList[1].x == -1)
				circle(canvas, strokes[i].cntrList[0], strokes[i].r, Scalar(strokes[i].color), -1, 8);
			int j = 1;
			while ((j < MAXSTROKELEN) && (strokes[i].cntrList[j].x != -1))
			{
				line(canvas, strokes[i].cntrList[j-1], strokes[i].cntrList[j], Scalar(strokes[i].color), strokes[i].r);
				j++;
			}
		}
	}
}