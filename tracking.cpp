#include "imageProcess.hpp"
//#include "poiDetection.hpp"
//#include "initializeNN.hpp"
//#include "find_word.hpp"
//#include "eye_detect.hpp"
#include <fstream>

void createTrackbars();
void on_trackbar(int, void*);
void createSynsetWord();
String inttostr(int input);

int N_MIN = 0;
int NUM_CLASSI = 9;

IplImage *mouthRegion;



int lipTracking() 
{
    
    ObjectDetector* faceDetector = new HaarCascadeObjectDetector("haarcascade_frontalface_alt.xml");
    createTrackbars();
    int key;
    IplImage *src;                                      // immagine presa dalla webcam
    IplImage *result;                                   // immagine che mostra il risultato del tracking
    Mat myMat;
    Mat croppedImage;
    Rect croppedRectangle;
    cvNamedWindow("result", CV_WINDOW_AUTOSIZE);

    CvCapture *capture = cvCaptureFromCAM(0);
    
    if (!capture) {
        cout << "WEBCAM ERROR: no signal \n";
        return -1;
    }
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480);
    
    src = cvQueryFrame(capture);
    result = cvCloneImage(src);  
    CvSize s = cvGetSize(src);
    int d = src->depth;
    int c = src->nChannels;
    mouthRegion=cvCreateImage(s, d, c); // (s, 8, 3)    
 
    result=cvCreateImage(s, d, c); // (s, 8, 3)   

    bool mouthFound = false;
    
    fstream f("train.txt", ios::out); //apre il file in scrittura (cancellando quello che già c'era

    if(!f) {
        cout<<"Impossibile aprire file di output!";
        return -1;
    }
    
    int photocount = 0; //initialize image counter
    String imagename;
    
    vector<int> compression_params;
    //vector that stores the compression parameters of the image
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    //specify the compression technique
    compression_params.push_back(100); //specify the compression quality
    
    while ((key = waitKey(30)) != 27 )
    {
        src = cvQueryFrame(capture);  
        cvCopy( src, result, NULL ); 
        
        /* check if mouth is visible in scene */
        try{
            findLip(src, faceDetector, mouthRegion, mouthFound);
        }
        catch(cv::Exception &e){cout << e.what() << endl;}
        
        if(!mouthFound)  continue;
        
        myMat = mouthRegion;
        croppedRectangle = Rect(300,300,100,100); //coordinate crop + width,height crop 
        croppedImage = myMat(croppedRectangle);
        /// display images ///
        resizeWindow("cropped", 100, 100);
        imshow("cropped", croppedImage);
        cvShowImage("result", result);
        
        imagename= "";
        if (key == 'c')
            {
                photocount++; // increment image number for each capture
                imagename = "images/image" + inttostr(photocount) + ".JPEG";
                cout << imagename << " " << N_MIN << endl;
                f << imagename << " " << N_MIN << endl;
                imwrite(imagename, croppedImage, compression_params);
            }
        
        
        
 

    }
    
    cvDestroyWindow("result");
    cvDestroyWindow("cropped");
   
    myMat.release();
    croppedImage.release();
    cvReleaseImage(&src);
    cvReleaseImage(&result);
    
    createSynsetWord();
    cvReleaseCapture(&capture);
    exit( 0 );
    cout << "QUI" << endl;
    return 0;
     
    }    


void on_trackbar(int, void*)
{//This function gets called whenever a
	// trackbar position is changed
}

void createTrackbars()
{
    String trackbarWindowName = "TrackBars";
    namedWindow(trackbarWindowName, WINDOW_AUTOSIZE);
    resizeWindow(trackbarWindowName, 300, 50);
    createTrackbar("n.Visema", trackbarWindowName, &N_MIN, NUM_CLASSI, on_trackbar);     

}

void createSynsetWord(){
    fstream f2("synset_word.txt", ios::out); //apre il file in scrittura (cancellando quello che già c'era
    for(int i=0; i<=NUM_CLASSI; i++){
        cout << i << " " << "viseme" << i << endl;
        f2 << i << " " << "viseme" << i << endl;
    }
        
}

String inttostr(int input)
{
       stringstream ss;
       ss << input;
       return ss.str();
}



