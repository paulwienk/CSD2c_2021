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
#include <thread>
#include "delay_circ_buffer/jackModuleAdapter.h"
#include "delay_circ_buffer/delayLine.h"
#include "delay_circ_buffer/delay.h"
#include "delay_circ_buffer/reverse.h"
#include "math.h"

// 10 seconds if sampleRate = 44100inBuf[i]
#define MAX_DELAY_SIZE 441000
#define DELAY_TIME_SEC 0.5f
#define REVERSE_TIME_SEC 1.0f

#define PI_2 6.28318530717959

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

            // store the Y and X coordinate in a variable, scaled to the right frequencies
            // the Y coordinate will be used to control the pitchsifter on the Y-axis (coordinates 150 to 600)
            // the X coordinate will be used to control the delay on the X-axis (coordinates 150 to 750)
            greenY = 6000.0 / (boundingBox[i].tl().y + 150.0);
            blueX = 7500.0 / (boundingBox[i].tl().x + 150.0);

            //cout << greenY << endl;

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

    // init portaudio audio callback
    const auto sampleRate = 44100;

    // retrieve either default or console line argument delaytime
    float delayTimeSec = DELAY_TIME_SEC;
    float reverseTimeSec = REVERSE_TIME_SEC;

    // transform delay inBuf[frames]time in seconds to delay time in number of samples
    unsigned int numSamplesDelay = sampleRate * delayTimeSec;
    unsigned int bufferSizeDelay = MAX_DELAY_SIZE * 2;

    unsigned int numSamplesReverse = sampleRate * reverseTimeSec;
    unsigned int bufferSizeReverse = numSamplesReverse * 2;

    // instantiate delay, 2x larger then delay time and set feedback/delay
    Delay delay(bufferSizeDelay, numSamplesDelay, sampleRate, 0.8);
    delay.log();
    Reverse reverse(bufferSizeReverse, sampleRate);

    JackModuleAdapter adapter;

    adapter.callback = [&reverse, &delay](float *inBuf, float *outBuf, unsigned int nframes) {

        for(unsigned int i = 0; i < nframes; i++) {
            // process reversing input
            reverse.process(inBuf, outBuf, i);
            delay.process(outBuf, outBuf, i);

        }
        return 0;
    };

    PortAudio portAudio { adapter };

    portAudio.setup(sampleRate, 64);

    auto callback = NoiseTestCallback {};
    auto portAudio1 = PortAudio { callback };
    auto blockSize = 64;

    portAudio1.setup(sampleRate, blockSize);

    VideoCapture cap(0);
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

        // will trigger the delay
        if (currentState)
        {
            delay.setFeedback(blueX);
            delay.setDelayTime(blueX);
        }

        // quit with escape button
        if (waitKey(30) == 27)
        {
            return 0;
        }
    }
}