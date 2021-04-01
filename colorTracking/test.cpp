#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// printing green and blue. the last detected color overrules the other color
class GreenBluePrinter {
public:
    void printBlue()
    {
        if (!blueLastPrinted) {
            greenLastPrinted = false;
            blueLastPrinted = true;
            cout << "blue object detected" << endl;
        }
    }

    void printRed()
    {
        if (!greenLastPrinted) {
            greenLastPrinted = true;
            blueLastPrinted = false;
            cout << "green object detected" << endl;
        }
    }

    bool greenLastPrinted = false;
    bool blueLastPrinted = false;
};

Mat frame;
GreenBluePrinter greenBluePrinter;

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

// HSV values of green and blue: {hueMin, saturationMin, valueMin, hueMax, saturationMax, valueMax}
vector<int> green   {50, 40, 86, 82, 200, 255};
vector<int> blue    {105, 74, 44, 135, 153, 255};

// color detection
void findColor(Mat object)
{
    Mat objectHSV, greenMask, blueMask;

    // RGB to HSV
    cvtColor(object, objectHSV, COLOR_BGR2HSV);

    // define values for each color
    Scalar greenMinHSV(green[0], green[1], green[2]);
    Scalar greenMaxHSV(green[3], green[4], green[5]);

    Scalar blueMinHSV(blue[0], blue[1], blue[2]);
    Scalar blueMaxHSV(blue[3], blue[4], blue[5]);

    // filters the green and blue object (mask)
    inRange(objectHSV, greenMinHSV, greenMaxHSV, greenMask);
    inRange(objectHSV, blueMinHSV, blueMaxHSV, blueMask);

    // get contours for each color with bounding box and print
    getContoursForColor(greenMask, Scalar (0, 255, 0), [] { greenBluePrinter.printRed(); });
    getContoursForColor(blueMask, Scalar (255, 0, 0), [] { greenBluePrinter.printBlue(); });
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