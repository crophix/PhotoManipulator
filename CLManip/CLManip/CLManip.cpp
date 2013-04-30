#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <cstring>
#include <cstdarg>

using namespace cv;
using namespace std;

const int MAX_ARGS = 15;

struct Method{
	char	name;
	int		kernel;
	int		scale;
};

void showHelp(char arg)
{
	cout << "Help:" << endl;
	cout << arg << endl;
	switch (arg)
	{
		case 'a': ;
		case 'g': cout << " -g: Gaussian blur" << endl;
			if (arg == 'g') break;
		case 'i': cout << " -i: Invert image colors" << endl;
			if (arg == 'i') break;
		case 'c': cout << " -c: Convert to grayscale" << endl;
			if (arg == 'c') break;
		case 'd': cout << " -d: Difference of Gaussian" << endl;
			if (arg == 'd') break;
		case 't': cout << " -t: Median Threshold" << endl;
			if (arg == 't') break;
		case 'm': cout << " -m: Median filter" << endl;
			if (arg == 'm') break;
		case 'e': cout << " -e: Equalize histogram" << endl;
			if (arg == 'e') break;
	}
}

Mat openImage(char* fname)
{
	Mat image;
	image = imread(fname, CV_LOAD_IMAGE_COLOR);

	return image;
}

void saveImage(Mat image, char* fname)
{
	imwrite(fname, image);
}

void saveCombinedImage(Mat img1, Mat img2, Mat img3)
{
	cvtColor(img2, img2, CV_GRAY2BGR);
	cvtColor(img3, img3, CV_GRAY2BGR);

	Mat final(img1.rows, 3*img1.cols, img1.type());
	Mat roiImgLeft		= final(Rect(0,0,img1.cols,img1.rows));
	Mat roiImgCenter	= final(Rect(img1.cols,0,img2.cols,img2.rows));
	Mat roiImgRight		= final(Rect(img1.cols+img2.cols,0,img3.cols,img3.rows));

	Mat roiImg1 = img1(Rect(0,0,img1.cols,img1.rows));
	Mat roiImg2 = img2(Rect(0,0,img2.cols,img2.rows));
	Mat roiImg3 = img3(Rect(0,0,img3.cols,img3.rows));

	roiImg1.copyTo(roiImgLeft);
	roiImg2.copyTo(roiImgCenter);
	roiImg3.copyTo(roiImgRight);

	saveImage(final, "a.jpg");
}

void showImages(int count, ...) //Display the provided Mat images
{
	va_list ap;
	va_start(ap, count);

	for (int i = 0; i < count; i++)
	{
		char intStr[2];
		char nmDest[32] = "Image ";
		sprintf(intStr, "%d", i+1);
		strcat(nmDest, intStr);
		namedWindow(nmDest, CV_WINDOW_AUTOSIZE);
		imshow(nmDest, va_arg(ap, Mat));  
	} 

	waitKey(0);			//Wait for keypress
}

Mat convertToGray(Mat image)
{
	if (image.channels() != 1)
		cvtColor(image, image, CV_BGR2GRAY);
	return image;
}

Mat gaussBlur(Mat image, int kernel)
{
	Mat blur;

	GaussianBlur(image, blur, Size(kernel, kernel),  0);
	return blur;
}

void invertImage(Mat image)
{
	int height, width, step, channels;
	uchar* data;

	IplImage img = image;
	height    = img.height;
	width     = img.width;
	step      = img.widthStep;
	channels  = img.nChannels;
	data      = (uchar *)img.imageData;
	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
			for(int k=0;k<channels;k++)
				data[i*step+j*channels+k]=255-data[i*step+j*channels+k];
}

int findMedian(Mat image)
{
	int histogram[256];
	for(int i = 0; i < 255; i++)
        histogram[i] = 0;
	for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++)
            histogram[(int)image.at<uchar>(y,x)]++;
	
	int numPixels = image.rows * image.cols;
	int count = 0;

	for (int i = 0; i < 256; i++)
	{
		count += histogram[i];
		if (count >= numPixels / 2)
			return i;
	}
	return 0;
}

Mat diffGauss(Mat origImg, Mat image, int kernel, int scale)
{
	Mat g1, g2, gray;
	g1 = gaussBlur(image, kernel);
	g2 = gaussBlur(image, scale);
	image = g1 - g2;
	//normalize(image, image, 256);

	//image = origImg + image;

	/*int min = 255, max = -255;
	for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++)
		{
			int p = (g1.at<uchar>(y,x) - g2.at<uchar>(y,x)) + origImg.at<uchar>(y,x);
			if (p > 255)
				p = 255;
			if (p < 0)
				p = 0;
			image.at<uchar>(y,x) = p;
			if (min > p)
				min = p;
			if (max < p)
				max = p;
		}
	/*for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++)
		{
			int p = (g1.at<uchar>(y,x) - g2.at<uchar>(y,x)) + origImg.at<uchar>(y,x);
			image.at<uchar>(y,x) =  (uchar)((255.0/(max - min)) * (p - min));
		}
	cout << min << " : " << max << endl;*/

	/*
	min = 255;
	max = -255;
	int m = findMedian(image);
	for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++)
		{
			int p = image.at<uchar>(y,x) + origImg.at<uchar>(y,x) -m;
			if (min > p)
				min = p;
			if (max < p)
				max = p;
		}
	for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++)
		{
			int p = image.at<uchar>(y,x) + origImg.at<uchar>(y,x) -m;
			image.at<uchar>(y,x) =  (uchar)(255.0/(max - min)) * (p - min);
		}
	cout << min << " : " << max << endl;*/
	return image;
}

Mat medianThreshold(Mat image)
{
	image = convertToGray(image);
	threshold(image, image, 1, 255, CV_THRESH_BINARY);
	//threshold(image, image, findMedian(image), 255, CV_THRESH_BINARY);
	return image;
}

Mat scalePixels(Mat image)
{
	int min = 255, max = -255;
	for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++)
		{
			int p = (int)image.at<uchar>(y,x);
			if (min > p)
				min = p;
			if (max < p)
				max = p;
		}
	cout << min << " : " << max << endl;

	return image;
}

int main( int argc, char** argv )
{
	if ((argc <= 1) || argv[1][1] == 'h')
	{
		if (argc == 3)
			showHelp(argv[2][1]);
		else showHelp('a');
		return 0;
	}

	Mat colorImage = openImage(argv[1]);
	Mat image = openImage(argv[1]);
	image = convertToGray(image);
	Mat finalImage = openImage(argv[1]);
	if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

	int chainLen = 0;				//The number of flags that are set, used for setting up the arguments
	Method mList[MAX_ARGS];
	for (int j=2; j < argc; j++)
	{
		if (argv[j][0] == '-') {
			mList[chainLen].name = argv[j][1];
			switch (argv[j][1])  //Adds the necessary arguments to the method
			{
			case 'g': 
				mList[chainLen].kernel = atoi(argv[++j]); 
				break;
			case 'i': 
				break;
			case 'c': 
				break;
			case 'd': 
				mList[chainLen].kernel = atoi(argv[++j]);
				mList[chainLen].scale = atoi(argv[++j]);
				break;
			case 't':
				break;
			case 'm':
				mList[chainLen].kernel = atoi(argv[++j]);
				break;
			case 'e':
				break;
			}
			chainLen++;
		} else break;
	}

	for (int i = 0; i < chainLen; i++)
	{
		Mat g1, g2;
		switch (mList[i].name)
		{
		case 'g':
			finalImage = gaussBlur(finalImage, mList[i].kernel);
			break;
		case 'i':
			invertImage(finalImage);
			break;
		case 'c': 
			finalImage = convertToGray(finalImage);
			break;
		case 'd':
			finalImage = convertToGray(finalImage);
			finalImage = diffGauss(image, finalImage, mList[i].kernel, mList[i].scale);
			scalePixels(finalImage);
			break;
		case 't':
			finalImage = medianThreshold(finalImage);
			break;
		case 'm':
			medianBlur(finalImage, finalImage, mList[i].kernel);
			break;
		case 'e':
			equalizeHist(finalImage, finalImage);
			break;
		}
	}

	saveCombinedImage(colorImage, image, finalImage);
	showImages(3, colorImage, image, finalImage);
	return 0;
}