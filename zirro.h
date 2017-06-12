#ifndef ZIRRO_ZIRRO_H
#define ZIRRO_ZIRRO_H

#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "bgsubcnt.h"
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace cv::bgsubcnt;
using namespace std;

class Zirro
{
private:
    Mat back;
    Mat fgMask; //fg mask
    Ptr<BackgroundSubtractor> pBgSub; //Background subtractor
    int maxFps;
    int maxFrames;
    int initFrames;
    double getImageDiff(Mat image);
    void actualizeBackground(Mat frame);
public:
    Zirro(int fps);
    bool sthInteresting(Mat frame);
    void destroy();
};

#endif
