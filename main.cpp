#include "zirro.h"

int main(int argc, char* argv[])
{

    VideoCapture capture("/home/jakub/Downloads/video4.avi");

    capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    capture.set(CV_CAP_PROP_FPS, 14);

    if(!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open video file" << endl;
        exit(EXIT_FAILURE);
    }

    Zirro z(14);

    Mat frame;
    while(true){
        if(!capture.read(frame)) {
            cerr << "Unable to read next frame." << endl;
            cerr << "Exiting..." << endl;
            z.destroy();
            exit(EXIT_FAILURE);
        }
        if(z.somethingInteresting(frame)){
            // decore qr codes
            for(string s: z.readQrCodes(frame)){
                cout << s << " ";
            }
            cout << endl;
            cout << z.somebodySmiling(frame) << endl;
        }


        waitKey(1000/14);

    }
    capture.release();
    return EXIT_SUCCESS;
}
