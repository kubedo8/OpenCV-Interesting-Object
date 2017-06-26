#include "zirro.h"

int main(int argc, char* argv[])
{

    VideoCapture capture("/home/jakub/Downloads/test1.avi");

    if(!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open video file" << endl;
        exit(EXIT_FAILURE);
    }

    Zirro z(14);

    Mat frame, resized;
    int numFrame = 0;
    while(true){
        if(!capture.read(frame)) {
            cerr << "Unable to read next frame." << endl;
            cerr << "Exiting..." << endl;
            z.destroy();
            exit(EXIT_FAILURE);
        }
        resize(frame, resized, Size(320, 180), 0, 0, INTER_CUBIC);
        if(z.somethingInteresting(resized)){

            cout << "Somebody is there!!!" << endl;

            if(numFrame % 10 == 5) {
                // decore qr codes
                for (string s: z.readQrCodes(frame)) {
                    cout << s << " ";
                }
                cout << endl;
//                string outputSmile;
//                if (z.somebodySmiling(frame)){
//                    outputSmile = "Smiliiiiing!!";
//                }else{
//                    outputSmile = "Not smiling :(";
//                }
//                cout << outputSmile << endl;
            }
            numFrame++;
        }else{
            numFrame = 0;
        }


        waitKey(20);

    }
    capture.release();
    return EXIT_SUCCESS;
}
