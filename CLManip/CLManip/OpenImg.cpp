#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv\cv.h>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file
	
	Mat grayImg;
	grayImg = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

	Mat convertedImg;
	cvtColor( image, convertedImg, CV_BGR2GRAY );

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Color Image", CV_WINDOW_AUTOSIZE );// Create a window for display.
	namedWindow( "Gray Image", CV_WINDOW_AUTOSIZE );
	namedWindow( "Converted Image", CV_WINDOW_AUTOSIZE );
    imshow( "Color Image", image );                   // Show our image inside it.
	imshow( "Gray Image", grayImg );
	imshow( "Converted Image", convertedImg );

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}