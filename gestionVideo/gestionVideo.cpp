#include "gestionVideo.h"

int capDev = 0;
VideoCapture cap;

extern "C" void *diffusion(void *ptr){

    int socket = *(int *)ptr;
    bool clientStopped = false;
    //OpenCV Code
    //----------------------------------------------------------

    Mat frame, frameGray;
    frame = Mat::zeros(480 , 640, CV_8UC1);   
     //make it continuous
    if (!frame.isContinuous()) {
        frame = frame.clone();
    }

    int frameSize = frame.total() * frame.elemSize();
    int bytes = 0;
    int key;
    

    //make img continuous
    if ( ! frame.isContinuous() ) { 
          frame = frame.clone();
          frameGray = frameGray = frame.clone();
    }


        
    std::cout << "Image Size:" << frameSize << std::endl;

    //TODO : appeler un thread qui attend qu'on demande une fin de diffusion
    //ce thread prend en argument l'adresse de la variable clientStopped, qui se met à 0 lorsqu'il arrête de regarder la diffusion.
    
    while(cap.isOpened() && !clientStopped) {
                
    /* get a frame from camera */
        cap >> frame;
        if (frame.empty())
        {
            cout << "Finished reading: empty frame" << endl;
            break;
        }
        //do video processing here 
        Mat image;
        frame.copyTo(image);
        cvtColor(image, frameGray, cv::COLOR_BGRA2GRAY);
        //On envoie l'image : 
        CHECK(send(socket, frameGray.data, frameSize, 0), "problème d'envoi");
    }

    close(socket);
    return NULL;
}

extern "C" int demarrerVideo(){
    //TODO : demander la caméra / capture d'écran à l'utilisateur
    int camera = 0;
    if(cap.open(camera)){
        return 1;
    }
    return 0;
}

extern "C" int arreterVideo(){
    cap.release();
}