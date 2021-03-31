#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// printing red and blue. the last detected color overrules the other color
class RedBluePrinter {
public:
    void printBlue()
    {
        if (!blueLastPrinted) {
            redLastPrinted = false;
            blueLastPrinted = true;
            cout << "blue object detected" << endl;
        }
    }

    void printRed()
    {
        if (!redLastPrinted) {
            redLastPrinted = true;
            blueLastPrinted = false;
            cout << "red object detected" << endl;
        }
    }

    bool redLastPrinted = false;
    bool blueLastPrinted = false;
};

Mat frame;
RedBluePrinter redBluePrinter;

// getting the contours of the object
void getContoursForColor(Mat object, Scalar color, function<void()> onDetected)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(object, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // contoursPoly will only get the corner points
    vector<vector<Point>> contoursPoly(contours.size());
    vector<Rect> boundingBox(contours.size());

    // area filtering, so it won't detect small objects
    for (int i = 0; i < contours.size(); i++)
    {
        int area = contourArea(contours[i]);


        if (area > 1000)
        {
            // find the corner points of the object and store them in contoursPoly colors
            float outline = arcLength(contours[i], true);
            approxPolyDP(contours[i], contoursPoly[i], outline * 0.05, true);

            // create and draw a bounding box around the object
            boundingBox[i] = boundingRect(contoursPoly[i]);
            rectangle(frame, boundingBox[i].tl(), boundingBox[i].br(), color, 3);

            onDetected();
        }
    }
}

// HSV values of red and blue: {hueMin, saturationMin, valueMin, hueMax, saturationMax, valueMax}
vector<int> red     {157, 74, 86, 179, 153, 255};
vector<int> blue    {105, 74, 44, 135, 153, 255};

// color detection
void findColor(Mat object)
{
    Mat objectHSV, redMask, blueMask;

    // RGB to HSV
    cvtColor(object, objectHSV, COLOR_BGR2HSV);

    // define values for each color
    Scalar redMinHSV(red[0], red[1], red[2]);
    Scalar redMaxHSV(red[3], red[4], red[5]);

    Scalar blueMinHSV(blue[0], blue[1], blue[2]);
    Scalar blueMaxHSV(blue[3], blue[4], blue[5]);

    // filters the red and blue object (mask)
    inRange(objectHSV, redMinHSV, redMaxHSV, redMask);
    inRange(objectHSV, blueMinHSV, blueMaxHSV, blueMask);

    // get contours for each color with bounding box and print
    getContoursForColor(redMask, Scalar (0, 255, 0), [] { redBluePrinter.printRed(); });
    getContoursForColor(blueMask, Scalar (255, 0, 0), [] { redBluePrinter.printBlue(); });
}


int main()
{
    VideoCapture cap(0);

    while (true)
    {
        cap.read(frame);
        findColor(frame);

        flip(frame, frame, 1);
        imshow("Image", frame);

        if (waitKey(30) == 27)
        {
            return 0;
        }
    }
}