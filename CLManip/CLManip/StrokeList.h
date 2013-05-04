#pragma once
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

struct brushStroke
{
	int		r;
	Point	cntr;
	Vec3b	color;
};

class StrokeList
{
public:
	StrokeList(void);
	~StrokeList(void);
	void addStroke(int, int, int, Mat);
	int getCount();
	void applyStrokes(Mat);
private:
	int count;
	brushStroke* strokes;
};