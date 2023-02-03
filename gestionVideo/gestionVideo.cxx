#include "gestionVideo.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>


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
        if(send(socket, frameGray.data, frameSize, 0) <= 0){
            clientStopped = true;
        }
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

extern "C" void *regarderDiffusion(void *ptr){
    int socket = *(int *)ptr;

    Mat img;
    img = Mat::zeros(480 , 640, CV_8UC1);    
    int imgSize = img.total() * img.elemSize();
    uchar *iptr = img.data;
    int bytes = 0;
    int key;

    //make img continuos
    if ( ! img.isContinuous() ) { 
          img = img.clone();
    }

    std::cout << "Image Size:" << imgSize << std::endl;


    namedWindow("Video stream",1);

    while (key != 'q') {

        if ((bytes = recv(socket, iptr, imgSize , MSG_WAITALL)) == -1) {
            std::cerr << "recv failed, received bytes = " << bytes << std::endl;
        }
        
        cv::imshow("CV Video Client", img); 
      
        if (key = cv::waitKey(10) >= 0) break;
    }   

    close(socket);

    return 0;
}