#include "zirro.h"

Zirro::Zirro(int fps) {
    maxFps = fps;
    maxFrames = 0;
    initFrames = maxFps * 10;
    pBgSub = createBackgroundSubtractorCNT();

    //create GUI windows
    namedWindow("Frame");
    namedWindow("Background");
    namedWindow("FG Mask");
}

bool Zirro::sthInteresting(Mat frame) {
    Mat grayFrame, diff;
    cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

    if(initFrames){
        cout << "Initializing" << "\n";
        actualizeBackground(grayFrame);
        initFrames--;
        return false;
    }

    bool isSthIntegersing;
    if(back.empty()){
        isSthIntegersing = false;
    }else{
        absdiff(back, grayFrame, diff);
        isSthIntegersing = getImageDiff(diff) > 0.1;
    }

    if(isSthIntegersing){
        if(maxFrames > 0){
            cout << "Skipping act: " << maxFrames << "\n";
            maxFrames--;
        }else{
            cout << "Applying" << "\n";

            actualizeBackground(grayFrame);
        }
    }else{
        cout << "Applying and resetting maxFrames" << "\n";

        actualizeBackground(grayFrame);
        maxFrames = maxFps * 20;
    }

    //show the current frame and the fg masks
    imshow("Frame", grayFrame);
    imshow("Background", back);
    imshow("FG Mask", fgMask);

    return isSthIntegersing;
}

void Zirro::destroy() {
    destroyAllWindows();
}

double Zirro::getImageDiff(Mat image) {
    int numPixels = image.cols*image.rows;
    int num = 0;
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            uchar intensity = image.at<uchar>(i,j);
            if(intensity > 15){
                num++;
            }
        }
    }

    return (double) num / (double) numPixels;
}

void Zirro::actualizeBackground(Mat frame) {
    pBgSub->apply(frame, fgMask);
    pBgSub.operator*().getBackgroundImage(back);
}

