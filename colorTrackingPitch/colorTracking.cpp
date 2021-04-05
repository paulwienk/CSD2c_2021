#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "port_audio.h"
#include <cmath>
#include <random>
#include <iostream>
#include <string>
#include "saw.h"
#include "circBuffer.h"

using namespace cv;
using namespace std;

//==================================================================================================================
constexpr double twoPi = 3.14159265359 * 2;

// generates a waveform that will be merged with the sawtooth
float windowing(float input) {
    input -= 0.5;
    input *= 0.5;

    return (float) std::cos(input * twoPi);

}

// the pitcher class returns 2 samples, 1 with a sawtooth and the windowing waveform
// and 1 which is the same but 180 degrees shifted
class Pitcher {
public:

    Pitcher() = default;

    float process(float input) {
        saw.tick();
        auto sawSample = saw.getSample();

        circBuffer1.setDistanceRW(sawSample * 4410);
        circBuffer2.setDistanceRW(std::fmod(sawSample + 0.5, 1) * 4410);

        circBuffer1.write(input);
        circBuffer2.write(input);

        auto sample1 = circBuffer1.read() * windowing(sawSample);
        auto sample2 = circBuffer2.read() * windowing(std::fmod(sawSample + 0.5, 1));

        circBuffer1.tick();
        circBuffer2.tick();


        return sample1 + sample2;


    }

    Saw saw{4, 44100};
    CircBuffer circBuffer1{44100};
    CircBuffer circBuffer2{44100};

};

class NoiseTestCallback : public AudioIODeviceCallback
{
public:

    void prepareToPlay (int sampleRate, int blockSize) override
    {
        std::cout << "starting callback\n";

    }

    // both channels are added together in the input buffer and mixed with the pitcher class
    void process (float* input, float* output, int numSamples, int numChannels) override
    {
        for (auto sample = 0; sample < numSamples; ++sample)
        {
            auto left = input[sample * numChannels];
            auto right = input[sample * numChannels + 1];
            auto in = (left + right) / 2;
            auto out = pitcher.process(in);

            output[sample * numChannels] = out;
            output[sample * numChannels + 1] = out;
        }
    }

    void releaseResources() override
    {
        std::cout << "stopping callback\n";
    }


    Pitcher pitcher;

};

// function to change the pitch in the commandline, real change happens at line 77
auto getNumericInput() {
    auto ans = std::string {};
    std::getline(std::cin, ans);
    return std::stod(ans);
}
//=================================================================================================================

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

            // printing coordinates of top left of the box
            cout << boundingBox[i].tl() << endl;

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

        if (waitKey(30) == 27)
        {
            return 0;
        }

        //callback.pitcher.saw.setFrequency(getNumericInput());

    }

}