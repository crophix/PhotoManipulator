#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>

#define _USE_MATH_DEFINES

#include <iostream>
#include <cstring>
#include <cstdarg>
#include <math.h>
#include "StrokeList.h"

using namespace cv;
using namespace std;

void showHelp(char arg)
{
	cout << "Help:" << endl;
	cout << arg << endl;
	switch (arg)
	{
		case 'a': ;
		case 'g': cout << " -g: Gaussian blur" << endl;
			cout << "     Takes a single argument for the kernel size which can be any whole number." << endl;
			if (arg == 'g') break;
		case 'i': cout << " -i: Invert image colors" << endl;
			cout << "     Will invert the color channels of the image." << endl;
			if (arg == 'i') break;
		case 'c': cout << " -c: Convert to grayscale" << endl;
			cout << "     Converts the image to grayscale using the built in OpenCV tool." << endl;
			if (arg == 'c') break;
		case 'C': cout << " -C: Salient preserving grayscale conversion" << endl;
			cout << "     Converts the image to grayscale using nonlinear mapping" << endl;
			if (arg == 'C') break;
		case 'd': cout << " -d: Difference of Gaussian" << endl;
			if (arg == 'd') break;
		case 't': cout << " -t: Threshold" << endl;
			cout << "     Takes a single argument for the threshold value." << endl;
			cout << "     Converts an image into a black and white image at the given threshold value." << endl;
			if (arg == 't') break;
		case 'm': cout << " -m: Median filter" << endl;
			if (arg == 'm') break;
		case 'e': cout << " -e: Equalize histogram" << endl;
			if (arg == 'e') break;
		case 'p': cout << " -p: Painterly style" << endl;
			if (arg == 'p') break;
		case 's': cout << " -s: Sharpen Image" << endl;
			if (arg == 's') break;
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

void saveCombinedImage(char* fname, int count, ...)
{
	// Combines some number of images together into a horizontal mosaic and saves it
	va_list ap;
	va_start(ap, count);

	Mat final;

	for (int i = 0; i < count; ++i)
	{
		Mat img = va_arg(ap, Mat);
		if (img.channels() == 1)
			cvtColor(img, img, CV_GRAY2BGR);
		if (i == 0)
			final = Mat(img.rows, count*img.cols, img.type());
		Mat roiImgFinal = final(Rect(i*img.cols, 0, img.cols, img.rows));
		Mat roiImg = img(Rect(0,0,img.cols,img.rows));
		roiImg.copyTo(roiImgFinal);
	}

	saveImage(final, fname);
}

void showImages(int count, ...) //Display the provided Mat images
{
	// Displays some number of images, each in it's own window
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
	// Converts a color image into a grayscale image if it is not one already
	if (image.channels() != 1)
		cvtColor(image, image, CV_BGR2GRAY);
	return image;
}

Mat gaussBlur(Mat image, int kernel)
{
	// Gaussian blur using the provided kernel size
	Mat blur;

	kernel = 2 * kernel + 1;
	GaussianBlur(image, blur, Size(kernel, kernel),  0);
	return blur;
}

void invertImage(Mat image)
{
	// Invert the provided image
	//    Only works on grayscale and RGB images
	if (image.channels() == 1)
		image = 255 - image;
	else
		image = Scalar(255, 255, 255) - image;
}

int findMedian(Mat image)
{
	// Returns the median value of the provided image
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

Mat diffGauss(Mat image, int k1, int k2)
{
	int c = 1;

	Mat g1, g2, pos, neg, final;
	g1 = gaussBlur(image, k1);
	g2 = gaussBlur(image, k2);
	/*
	pos = c*(g2 - g1);
	invertImage(g1);
	invertImage(g2);
	neg = c*(g2 - g1);
	
	final = image + pos;
	final = image - neg;*/

	absdiff(g2, g1, pos);
	final = image + pos;
	return pos;
}

Mat threshold(Mat image, int t)
{
	image = convertToGray(image);
	threshold(image, image, t, 255, CV_THRESH_BINARY);
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

Mat difference(Mat img1, Mat img2)
{
	Mat diff = Mat_<double>(img1.rows, img1.cols);

	for(int y = 0; y < img1.rows; y++)
	{
        for(int x = 0; x < img1.cols; x++)
		{
			Vec3b a = img1.at<Vec3b>(y,x);
			Vec3b b = img2.at<Vec3b>(y,x);

			diff.at<double>(y,x) = pow(((a.val[0]-b.val[0])*(a.val[0]-b.val[0]))
									+((a.val[1]-b.val[1])*(a.val[1]-b.val[1]))
									+((a.val[2]-b.val[2])*(a.val[2]-b.val[2])), 0.5);
		}
	}
	return diff;
}

void paintLayer(Mat canvas, Mat referenceImage, int brush)
{
	int T	= 35;	// Threshold for if the area should have a stroke added
	float f = 1.0; // A scaling factor for the gridSize 

	StrokeList S = StrokeList(referenceImage, canvas, brush);
	Mat diff = difference(canvas, referenceImage);

	int gridSize = (int)(f* brush);

	for (int y = 0; y < canvas.rows; y += gridSize)
	{
		for(int x = 0; x < canvas.cols; x += gridSize)
		{
			int count = 0;
			double areaError = 0.0;
			for (int i = max(0, y - (gridSize/2)); i < min(canvas.rows, y + (gridSize/2)); ++i)
			{
				for (int j = max(0, x - (gridSize/2)); j < min(canvas.cols, x + (gridSize/2)); ++j)
				{
					areaError += diff.at<double>(i,j); // Total the difference in the grid
					count++;
				}
			}
			areaError = areaError / count;
			if (areaError > T) {
				S.addCurvedStroke(brush, x, y, referenceImage, canvas);
			}
		}
	}
	S.applyCurvedStrokes(canvas);
}

Mat paintImage(Mat orig, int count, ...)
{
	va_list ap;
	va_start(ap, count);
	Mat referenceImage;

	Mat canvas = orig.clone();
	//canvas = Scalar(100, 100, 100);
	invertImage(canvas);

	// paint a layer for each brush size provided
	for (int i = 0; i < count; i++)
	{
		int brush = va_arg(ap, int);
		cout << brush << endl;
		int kernel = (brush % 2 == 0)? brush+1 : brush;

		referenceImage = gaussBlur(orig, kernel);
		paintLayer(canvas, referenceImage, brush);
	}
	return canvas;
}

Mat sharpenImage(Mat orig)
{
	Mat sharpImg;
	int p = 10; // sets the emphasis placed on the edges

	Mat g1 = gaussBlur(orig, 1);
	Mat g2 = gaussBlur(orig, 5);
	sharpImg = ((p+1)*g1) - (p*g2);

	return sharpImg;
}

Mat salientGrayscale(Mat image)
{
	// Implements "Robust Color-to-gray via Nonlinear Mapping"

	// paramaters
	double	alpha = 1.0; // Controls the emphasis on the color difference
	int		n = 4;

	Mat final;
	cvtColor(image, final, CV_BGR2Lab); // convert to L*a*b* color space

	// Compute Sobel derivatives
	Mat greyImg, xImg, yImg;
	cvtColor(image, greyImg, CV_BGR2GRAY);
	Sobel(greyImg, xImg, CV_16S, 1, 0);
	Sobel(greyImg, yImg, CV_16S, 0, 1);



	cvtColor(final, final, CV_Lab2BGR); // convert back to BGR color space
	return final;
}

Mat fastLinearGrayscale(Mat image)
{
	/* This is a linear greyscale mapping base on the following papers:

	Lu, Chewu, et al. 2012 Real-time Contrast Preserving Decolorization.
	Colo2Gray: Salience-Preserving Color Removal. Gooch et al. SIGGRAPH 05
	*/
	Mat final;
	cvtColor(image, final, CV_BGR2GRAY);

	//Generate the pool of pixel pairs and compute the color difference between each pair
	int alpha = 15;
	Mat labImg;
	cvtColor(image, labImg, CV_BGR2Lab);
	struct Pool {
		Point	p1;
		Point	p2;
		uchar	d;
	}pool[64*64];
	for (int i = 0; i < 64*64; ++i)
	{
		pool[i].p1 = Point(rand()%image.cols, rand()%image.rows);
		pool[i].p2 = Point(rand()%image.cols, rand()%image.rows);
		Vec3b a = labImg.at<Vec3b>(pool[i].p1);
		Vec3b b = labImg.at<Vec3b>(pool[i].p2);
		double x = pow(((a.val[1]-b.val[1])*(a.val[1]-b.val[1]))
					  +((a.val[2]-b.val[2])*(a.val[2]-b.val[2])),0.5);
		pool[i].d = alpha * tanh(x/alpha);
	}

	// Find the linear grayscale transformation that maximizes the effect of color differences
	double bestwb = 1.0, bestwg = 0.0, bestwr = 0.0;
	double bestTotal = 0.0;
	double wb = 1.0, wg = 0.0, wr = 0.0;

	for (int b = 10; b >= 0; b -= 1)
	{
		for (int g = 0; g <= 10-b; g++)
		{
			wb = b / 10.0;
			wg = g / 10.0;
			wr = 1.0 - (wg + wb);
			double sum = 0.0;
			for (int i = 0; i < 64*64; ++i)
			{
				Vec3b a = image.at<Vec3b>(pool[i].p1);
				Vec3b b = image.at<Vec3b>(pool[i].p2);
				uchar g1 = (uchar) ((a.val[0]*wb) + (a.val[1]*wg) + (a.val[2]*wr));
				uchar g2 = (uchar) ((b.val[0]*wb) + (b.val[1]*wg) + (b.val[2]*wr));
				if (abs(g1-g2) > pool[i].d)
					sum += abs(g1-g2);
				else
					sum += pool[i].d;
			}
			if (bestTotal == 0.0) bestTotal = sum;
			if (bestTotal >= sum) 
			{
				bestTotal = sum;
				bestwb = wb;
				bestwg = wg;
				bestwr = wr;
				//cout  << sum << " : " << wb << " : " << wg << " : " << wr << endl;
			}
		}
	}

	for (int y = 0; y < final.rows; y++)
	{
		for(int x = 0; x < final.cols; x++)
		{
			Vec3b color = image.at<Vec3b>(y,x);
			final.at<uchar>(y,x) = (uchar) ((color.val[0]*bestwb) + (color.val[1]*bestwg) + (color.val[2]*bestwr));
		}
	}
	
	return final;
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
	Mat finalImage = openImage(argv[1]);
	if(! colorImage.data )                          // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
	int k1, k2;
	for (int j=2; j < argc; j++)
	{
		if (argv[j][0] == '-') {
			switch (argv[j][1])  //Adds the necessary arguments to the method
			{
			case 'g': // Gaussian blur with a single filter
				finalImage = gaussBlur(finalImage, atoi(argv[++j]));
				break;
			case 'i':
				invertImage(finalImage);
				break;
			case 'c': // Simple greyscale conversion using the built-in OpenCV tool
				finalImage = convertToGray(finalImage);
				break;
			case 'C': // Salient preserving Greyscale conversion
				finalImage = salientGrayscale(finalImage);
				break;
			case 'l':
				finalImage = fastLinearGrayscale(finalImage);
				break;
			case 'd': 
				k1 = atoi(argv[++j]);
				k2 = atoi(argv[++j]);
				finalImage = diffGauss(colorImage, k1, k2);
				break;
			case 't':
				finalImage = threshold(finalImage, atoi(argv[++j]));
				break;
			case 'm':
				medianBlur(finalImage, finalImage, atoi(argv[++j]));
				break;
			case 'e':
				equalizeHist(finalImage, finalImage);
				break;
			case 'p':
				finalImage = paintImage(finalImage, 3, 8, 4, 2);
				break;
			case 's': // Sharpen image using XDoG
				finalImage = sharpenImage(finalImage);
				break;
			}
		} else break;
	}

	//saveCombinedImage("sharpPaintFace.jpg", 2, colorImage, finalImage);
	//saveImage(finalImage, "linGrey.jpg");
	showImages(2, colorImage, finalImage);
	return 0;
}