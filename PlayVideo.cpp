#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define YELLOW_MIN_HUE 50
#define YELLOW_MAX_HUE 60
#define THRESH_BINARY_INVERT 1
const char* WIN_ORIGINAL = "original";
const char* WIN_CALCULATE = "calculate";
const int erosion_size = 1;


int main()
{
    VideoCapture stream0("rpi_record.mp4");
    Mat img_src, img_hsv, img_erosion;
	vector<Mat> channels(3);
    vector<KeyPoint> key_points;
	cv::SimpleBlobDetector::Params blob_detect_params;
	
	blob_detect_params.filterByArea = true;
	blob_detect_params.minArea = 1500;
	blob_detect_params.filterByCircularity = false;
	blob_detect_params.filterByConvexity = false;
	blob_detect_params.minDistBetweenBlobs = 50.0f;
	
	Ptr<SimpleBlobDetector> blob_detector = 
	                   SimpleBlobDetector::create(blob_detect_params);
	   
	if(!stream0.isOpened())
	{
		cout << "Cannot open video stream!" << endl ;
		return -1;
	}

	Size refS = Size((int)stream0.get(CV_CAP_PROP_FRAME_WIDTH),
	                 (int)stream0.get(CV_CAP_PROP_FRAME_HEIGHT));
	const int start_pos = 100;

	namedWindow(WIN_ORIGINAL, WINDOW_NORMAL);
	namedWindow(WIN_CALCULATE, WINDOW_NORMAL);
	cvMoveWindow(WIN_ORIGINAL, start_pos, 0);
	cvMoveWindow(WIN_CALCULATE, start_pos + refS.width, 0);

	int iLowH = 20;
	int iHighH = 30;

	int iLowS = 100;
	int iHighS = 255;
	 
	int iLowV = 100;
	int iHighV = 255;

    /* Hue lower and higer bound trackbar*/
    cvCreateTrackbar("LowH", WIN_CALCULATE, &iLowH, 179);
    cvCreateTrackbar("HighH", WIN_CALCULATE, &iHighH, 179);

    /* Saturation lower and higer bound trackbar*/
    cvCreateTrackbar("LowS", WIN_CALCULATE, &iLowS, 255);
    cvCreateTrackbar("HighS", WIN_CALCULATE, &iHighS, 255);

    /* Value lower and higer bound trackbar*/
    cvCreateTrackbar("LowV", WIN_CALCULATE, &iLowV, 255);
    cvCreateTrackbar("HighV", WIN_CALCULATE, &iHighV, 255);

    Mat erosion_element = getStructuringElement(MORPH_RECT,
	                       Size(2*erosion_size+1, 2*erosion_size+1),
						   Point(erosion_size, erosion_size));
    while(true)
	{
		bool bSuccess = stream0.read(img_src);
		if(!bSuccess)
		{
            cout << "play video done" << endl;
			break;
	    }

		cvtColor(img_src, img_hsv, CV_BGR2HSV);
        
		split(img_hsv, channels);
		
		Mat img_threshold(img_hsv.rows, img_hsv.cols, CV_8UC1);

        inRange(img_hsv, Scalar(iLowH, iLowS, iLowV), 
		                 Scalar(iHighH, iHighS, iHighV), 
						 img_threshold);
        
		erode(img_threshold, img_erosion, erosion_element);

        blob_detector->detect(img_erosion, key_points);
		drawKeypoints(img_erosion, key_points, img_erosion, Scalar(0,0,255), 
		              DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

		imshow(WIN_CALCULATE, img_erosion);
		imshow(WIN_ORIGINAL, img_src);

		if(waitKey(0) >= 0)
		{
			continue;
		}
    }

	return 0;
}
