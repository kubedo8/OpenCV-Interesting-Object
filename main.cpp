#include "zirro.h"

int main(int argc, char* argv[])
{

    VideoCapture capture("/home/jakub/Downloads/video2.avi");

    capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    if(!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open video file" << endl;
        exit(EXIT_FAILURE);
    }

    Zirro z(10);

    Mat frame;
    while(true){
        if(!capture.read(frame)) {
            cerr << "Unable to read next frame." << endl;
            cerr << "Exiting..." << endl;
            exit(EXIT_FAILURE);
        }
        if(z.sthInteresting(frame)){
            // TODO call some methods
        }

        waitKey(10);

    }
    z.destroy();
    capture.release();
    return EXIT_SUCCESS;
}
