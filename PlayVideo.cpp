#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define YELLOW_MIN_HUE 50
#define YELLOW_MAX_HUE 60

int main()
{
    VideoCapture stream0("rpi_record.mp4");
    Mat img;
	Mat img_hsv;

	if(!stream0.isOpened())
	{
		cout << "Cannot open video stream!" << endl ;
		return -1;
	}

	namedWindow("video", WINDOW_NORMAL);

    while(true)
	{
		stream0.read(img);
		cvtColor(img, img_hsv, CV_BGR2HSV);

		Mat distance_hue_img(img_hsv.rows, img_hsv.cols, CV_8UC1);

		
		imshow("video", img);
		if(waitKey(30) >= 0)
		{
			break;
		}
	}
	return 0;
}
