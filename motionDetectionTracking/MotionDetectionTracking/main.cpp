#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
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

	Mat frameOld, frameNew, diff, gray, blur, thresh, dilated;

	webcam.read(frameOld);
	webcam.read(frameNew);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	while (webcam.isOpened())
	{
		// update current frame
		webcam >> frameNew;

		// difference between first and second frame
		absdiff(frameOld, frameNew, diff);

		// threshold
		threshold(blur, thresh, 50, 255, THRESH_BINARY);

		// make image gray and blur it
		cvtColor(diff, gray, COLOR_BGR2GRAY);
		GaussianBlur(gray, blur, Size(3, 3), 3, 0);

		// dilate
		Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
		dilate(blur, dilated, kernel);

		// findContours
		findContours(dilated, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		drawContours(dilated, contours, -1, Scalar(255, 0, 255), 2);

		// calc area of motion
		for (int i = 0; i < contours.size(); i++)
		{
			int area = contourArea(contours[i]);
			if (area > 50 && area <= 20000) {
				cout << "Area: " << i << " " << area << endl;
			}
		}

		imshow("Webcam", frameNew);
		imshow("diff", diff);
		imshow("dilated", dilated);

		webcam >> frameOld;

		// wait (1ms) for key to pressed
		if (waitKey(1) >= 0)
			break;
	}
	return 0;
}