#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
//#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

///////////////// Motion Detection and Tracking //////////////////////

int main()
{
	// open the first webcam plugged in the computer
	VideoCapture webcam(0);
	if (!webcam.isOpened()) {
		std::cerr << "ERROR: Could not open camera" << std::endl;
		return 1;
	}

	// create a window to display the images from the webcam
	cv::namedWindow("Webcam", WINDOW_AUTOSIZE);

	Mat frame, background, currentFrame, diff, thresh, erode, dilated;

	webcam.read(background);
	webcam.read(frame);

	// make image gray and blur it
	cvtColor(frame, background, COLOR_BGR2GRAY);
	GaussianBlur(background, background, Size(21, 21), 0);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	while (webcam.read(frame))
	{
		Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));

		// make image gray and blur it
		cvtColor(frame, currentFrame, COLOR_BGR2GRAY);
		GaussianBlur(currentFrame, currentFrame, Size(21, 21), 0);

		// difference between first and second frame
		absdiff(background, currentFrame, diff);

		// threshold
		threshold(diff, thresh, 25, 255, THRESH_BINARY);

		// dilate
		dilate(thresh, dilated, kernel);

		// findContours
		findContours(dilated, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		drawContours(dilated, contours, -1, Scalar(255, 0, 255), 2);

		// calc area of motion
		for (int i = 0; i < contours.size(); i++)
		{
			int area = contourArea(contours[i]);
			if (area > 50 && area <= 20000) {
				cout << "Area: " << i << " " << area << endl;
			}
		}

		//boundingRect();

		imshow("Webcam", frame);
		imshow("diff", diff);
		imshow("threshhold", thresh);
		imshow("dilate", dilated);
		//imshow("erode", erode);

		// update current frame
		//webcam >> frame;

		// wait (1ms) for key to pressed
		if (waitKey(1) >= 0)
			break;
	}
	return 0;
}