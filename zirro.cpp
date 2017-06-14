#include "zirro.h"

const double Zirro::treshold = 0.1;
const int Zirro::imageDiffTreshold = 15;
const int Zirro::minSecsBackground = 1;
const int Zirro::maxSecsBackground = 60;
const string Zirro::faceCascadeName = "/home/jakub/OpenCV/opencv-3.2.0/data/haarcascades/haarcascade_frontalface_default.xml";
const string Zirro::smileCascadeName = "/home/jakub/OpenCV/opencv-3.2.0/data/haarcascades/haarcascade_smile.xml";

Zirro::Zirro(int fpsInit) {
    initFrames = fpsInit * minSecsBackground;
    currentFrames = 0;
    bgSub = createBackgroundSubtractorCNT(initFrames, true, fpsInit * maxSecsBackground);
    qrCodeScanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);

    if (!faceClassifier.load(faceCascadeName)) {
        printf("Error loading face cascade\n");
    };

    if (!smileClassifier.load(smileCascadeName)) {
        printf("Error loading smile cascade\n");
    };


    //create GUI windows
    namedWindow("Frame");
    namedWindow("Background");
    namedWindow("FG Mask");
}

bool Zirro::somethingInteresting(Mat frame) {
    Mat grayFrame;
    cvtColor(frame, grayFrame, COLOR_BGR2GRAY);
    currentFrames++;
    actualizeBackground(grayFrame);

    bool isSthIntegersing;
    if (currentFrames <= initFrames) {
        isSthIntegersing = false;
    } else {
        Mat diff;
        absdiff(currentBackground, grayFrame, diff);
        double d = getImageDiff(diff);
        isSthIntegersing = d > treshold;

        cout << d << endl;
    }

    showFrames(grayFrame);

    return isSthIntegersing;
}

void Zirro::destroy() {
    destroyAllWindows();
}

vector<string> Zirro::readQrCodes(Mat frame) {
    Mat grayFrame;
    cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

    uint width = (uint) grayFrame.cols;
    uint height = (uint) grayFrame.rows;
    uchar *raw = grayFrame.data;

    // Wrap image data
    Image image(width, height, "Y800", raw, width * height);

    // Scan the image for barcodes
    qrCodeScanner.scan(image);

    SymbolSet s = image.get_symbols();
    vector<string> symbols;
    for (SymbolIterator symbol = s.symbol_begin(); symbol != s.symbol_end(); ++symbol) {
        symbols.push_back(symbol.operator*().get_data());
    }
    return symbols;
}

bool Zirro::somebodySmiling(Mat frame) {
    Mat grayFrame;
    cvtColor(frame, grayFrame, COLOR_BGR2GRAY);
    equalizeHist(grayFrame, grayFrame);

    vector<Rect> faces;
    // detect faces
    faceClassifier.detectMultiScale(grayFrame, faces, 1.1, 2, CV_HAAR_SCALE_IMAGE, Size(30, 30));

    for( int i = 0; i < faces.size(); i++ ) {
        Mat faceROI = grayFrame( faces[i] );
        vector<Rect> smile;
        // detect smiles
        smileClassifier.detectMultiScale(faceROI, smile, 1.1, 2,  CV_HAAR_SCALE_IMAGE, Size(20, 20));
        return !smile.empty();

    }

    return false;
}

double Zirro::getImageDiff(Mat image) {
    int numPixels = image.cols * image.rows;
    int num = 0;
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            uchar intensity = image.at<uchar>(i, j);
            if (intensity > imageDiffTreshold) {
                num++;
            }
        }
    }

    return (double) num / (double) numPixels;
}

void Zirro::actualizeBackground(Mat frame) {
    bgSub->apply(frame, fgMask);
    bgSub.operator*().getBackgroundImage(currentBackground);
}

void Zirro::showFrames(Mat frame) {
    //show the current frame and the fg masks
    imshow("Frame", frame);
    imshow("Background", currentBackground);
    imshow("FG Mask", fgMask);
}
