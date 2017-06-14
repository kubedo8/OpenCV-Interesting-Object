#ifndef ZIRRO_ZIRRO_H
#define ZIRRO_ZIRRO_H

#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include "bgsubcnt.h"
#include "zbar.h"

#include <iostream>

using namespace cv;
using namespace cv::bgsubcnt;
using namespace std;
using namespace zbar;

class Zirro
{
private:

    static const double treshold;
    static const int imageDiffTreshold;
    static const int minSecsBackground;
    static const int maxSecsBackground;
    static const string faceCascadeName;
    static const string smileCascadeName;

    CascadeClassifier faceClassifier;
    CascadeClassifier smileClassifier;

    Mat currentBackground;
    Mat fgMask; //fg mask
    Ptr<BackgroundSubtractor> bgSub; //Background subtractor
    int initFrames;
    int currentFrames;
    ImageScanner qrCodeScanner;

    double getImageDiff(Mat image);
    void actualizeBackground(Mat frame);
    void showFrames(Mat frame);

public:
    Zirro(int fps);
    bool somethingInteresting(Mat frame);
    bool somebodySmiling(Mat frame);
    vector<string> readQrCodes(Mat frame);
    void destroy();
};

#endif
