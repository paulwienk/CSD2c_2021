
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/video/tracking.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	// opening camera
	VideoCapture cap(0);
	
	while (true)
	{
		Mat frame;
		Mat frameHSV;
		Mat outputRedHSV;
		Mat outputBlueHSV;
		Mat red;
		Mat blue;

		cap.read(frame);

		// converting RGB to HSV
		cvtColor(frame, frameHSV, COLOR_BGR2HSV);

		// HSV values of red
		// first int = minimum and maximum of the hue (color)
		// second int = min and max of the saturation/brightness
		// third int = idk
		vector<int> min_red = { 166, 150, 84 };
		vector<int> max_red = { 179, 255, 255 };

        // HSV values of blue
        vector<int> min_blue = { 100, 150, 20 };
        vector<int> max_blue = { 140, 255, 255 };

		// only displaying the red and blue object as white
		inRange(frameHSV, min_red, max_red, outputRedHSV);
        inRange(frameHSV, min_blue, max_blue, outputBlueHSV);

		// converting the above output to the original (red and blue) color
		bitwise_and(frame, frame, red, outputRedHSV);
        bitwise_and(frame, frame, blue, outputBlueHSV);

		// display normal camera and camera with displaying only red objects
		imshow("Camera", frame);
		imshow("Red", red);
		imshow("Blue", blue);

		// if esc button is pressed for longer than 30 ms, then quit
		if (waitKey(30) == 27)
		{
			return 0;
		}


	}
}

