#pragma once
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

const int	MAXSTROKELEN = 16;
const int	MINSTROKELEN = 4;

struct curvedStroke
{
	int		r;
	Point	cntrList[MAXSTROKELEN];
	Vec3b	color;
};

class StrokeList
{
public:
	StrokeList(Mat, Mat, int);
	~StrokeList(void);
	void addCurvedStroke(int, int, int, Mat, Mat);
	void applyCurvedStrokes(Mat);
private:
	int				maxStrokes;
	curvedStroke*	strokes;
	Mat				xImg;
	Mat				yImg;
	Mat				diff;
	int				count;
};