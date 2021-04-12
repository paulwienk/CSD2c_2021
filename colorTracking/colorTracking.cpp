#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <random>
#include <iostream>
#include <string>
#include "port_audio.h"
#include "saw.h"
#include "circBuffer.h"
#include "pitcher.h"

using namespace cv;
using namespace std;

Mat frame;

// green will represent the pitchshifter, blue will represent the delay
float greenY;
float blueX;

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

            // store the Y and X coordinate in a variable
            // the Y coordinate will be used to control the pitchsifter on the Y-axis (150 to 600)
            // the X coordinate will be used to control the delay on the X-axis (150 to 750)
            greenY = 6000.0 / (boundingBox[i].tl().y + 150.0);
            blueX = 7500.0 / (boundingBox[i].tl().x + 150.0);

            cout << greenY << endl;

            onDetected();
        }
    }
}

// define current state of the visible object (green or blue)
bool currentState;

// printing green and blue. the last detected color overrules the other color
class GreenBlueSelector {
public:

    void selectGreen()
    {
        if (!greenLastPrinted) {

            greenLastPrinted = true;
            blueLastPrinted = false;
            cout << "green object detected" << endl;
            currentState = false;
        }
    }

    void selectBlue()
    {
        if (!blueLastPrinted) {

            greenLastPrinted = false;
            blueLastPrinted = true;
            cout << "blue object detected" << endl;
            currentState = true;
        }
    }

    bool greenLastPrinted = false;
    bool blueLastPrinted = false;
};

GreenBlueSelector greenBluePrinter;

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
    getContoursForColor(greenMask, Scalar (0, 255, 0), [] { greenBluePrinter.selectGreen(); });
    getContoursForColor(blueMask, Scalar (255, 0, 0), [] { greenBluePrinter.selectBlue(); });
}


int main()
{
    auto callback = NoiseTestCallback {};
    auto portAudio = PortAudio { callback };

    auto sampleRate = 44100;
    auto blockSize = 64;

    VideoCapture cap(0);

    portAudio.setup(sampleRate, blockSize);

    while (true)
    {
        cap.read(frame);
        findColor(frame);

        flip(frame, frame, 1);
        imshow("Image", frame);

        // triggers the pitchshifter
        if (!currentState)
        {
            callback.pitcher.saw.setFrequency(greenY);
        }

        // trigger the delay
        if (currentState)
        {
            callback.pitcher.saw.setFrequency(blueX);
        }

        // quit with escape button
        if (waitKey(30) == 27)
        {
            return 0;
        }
    }
}