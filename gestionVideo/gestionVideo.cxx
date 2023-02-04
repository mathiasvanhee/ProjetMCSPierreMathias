#include "gestionVideo.h"



int isCapturing = 0;
pthread_t affichagevideo_tid;
VideoCapture cap;

void *diffusion(void *ptr){
    struct sockaddr_in clt;
    long param = (long) ptr;
    int socket = (int)param;
    bool clientStopped = false;

    char buffer[10];

    socklen_t cltLen = sizeof(clt);
    recvfrom(socket, buffer, 10, 0, (struct sockaddr *)&clt, &cltLen);
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


    //TODO : appeler un thread qui attend qu'on demande une fin de diffusion
    //ce thread prend en argument l'adresse de la variable clientStopped, qui se met à 0 lorsqu'il arrête de regarder la diffusion.

    while(isCapturing) {
                
    /* get a frame from camera */
        cap >> frame;
        if (frame.empty())
        {
            continue;
        }
        //do video processing here 
        Mat image;
        frame.copyTo(image);
        cvtColor(image, frameGray, cv::COLOR_BGRA2GRAY);
        //On envoie l'image : 
        // if(send(socket, frameGray.data, frameSize, 0) <= 0){
        //     clientStopped = true;
        // }
        //nbOctets = sendto(sock, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)addr, sizeof(addr)), "Pb-sendto")
        //send processed image
        if ((bytes = sendto_bigbuffer(socket, frameGray.data, frameSize, 0, (struct sockaddr *)&clt, cltLen)) < 0){
                std::cerr << "bytes = " << bytes << std::endl;
                perror("pb envoi image : ");
                break;
        }
        waitKey(10);
    }

    close(socket);
    pthread_exit(NULL);
}

int demarrerVideo(){
    //TODO : demander la caméra / capture d'écran à l'utilisateur
    int camera = 0;
    
    if(cap.open(camera)){
        isCapturing = 1;
        pthread_create(&affichagevideo_tid, NULL, afficherDiffusion, NULL);
        return 1;
    }
    return 0;
}

int arreterVideo(){
    isCapturing = 0;
    cap.release();
    //pthread_join(affichagevideo_tid,NULL);
    return 1;
}

void* regarderDiffusion(void *ptr){
    long param = (long) ptr;
    int socket = (int)param;
    struct sockaddr_in clt;
    socklen_t cltLen = sizeof(clt);

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

    namedWindow("Video stream : S pour stopper",1);
    while (key != 's') {
        if ((bytes = recvfrom_bigbuffer(socket, iptr, imgSize , MSG_WAITALL, (struct sockaddr *)&clt, &cltLen)) == -1) {
            break;
        }
        
        cv::imshow("Video stream : S pour stopper", img);
        if (key = cv::waitKey(17) >= 0) break;
    }   

    destroyAllWindows();
    close(socket);

    pthread_exit(NULL);
}

void* afficherDiffusion(void *p){
    Mat frame;
    frame = Mat::zeros(480 , 640, CV_8UC1);   
     //make it continuous
    if (!frame.isContinuous()) {
        frame = frame.clone();
    }

    int frameSize = frame.total() * frame.elemSize();
    int bytes = 0;
    int key;
    

    namedWindow("diffuseur",1);

    while(cap.isOpened() && isCapturing == 1){
        cap >> frame;
        if (frame.empty())
        {
            break;
        }
        cv::imshow("diffuseur", frame);
        waitKey(10);
    }

    destroyAllWindows();
    pthread_exit(NULL);
}


int sendto_bigbuffer(int sock, const void *buffer, const size_t buflen, int flags,
                     const struct sockaddr *dest_addr, socklen_t addrlen)
{
    size_t sendlen = MIN(buflen, 1024);
    size_t remlen  = buflen;
    const void *curpos = buffer;

    while (remlen > 0)
    {
        ssize_t len = sendto(sock, curpos, sendlen, flags, dest_addr, addrlen);
        if (len == -1)
            return -1;

        curpos += len;
        remlen -= len;
        sendlen = MIN(remlen, 1024);
    }

    return buflen;
}


int recvfrom_bigbuffer(int sock, void *buffer, size_t buflen, int flags,
                       struct sockaddr *src_addr, socklen_t *addrlen)
{
    size_t recvlen = MIN(buflen, 1024);
    size_t remlen = buflen;
    void *curpos = buffer;
    while (remlen > 0)
    {
        ssize_t len = recvfrom(sock, curpos, recvlen, flags, src_addr, addrlen);
        if (len == -1)
            return -1;

        curpos += len;
        remlen -= len;
        recvlen = MIN(remlen, 1024);
    }

    return buflen;
}