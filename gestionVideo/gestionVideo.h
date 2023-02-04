//#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>


#ifndef PAUSE
#define PAUSE(msg) printf("%s [touche]", msg); getchar();
#endif

using namespace cv;
using namespace std;

typedef struct infosConnexion
{
    int sock;
    struct sockaddr_in * addrSrc;
}infosConnexion_t;


/**
 * \brief       Diffuse la capture de vidéo à un client.
 * \param ptr   Adresse de la socket de dialogue avec le client.
 * \note        A utiliser en tant que thread .
 * \warning     La fonction demarrerVideo doit avoir été appelée avant.
 * \warning     la socket doit être bien initialisée
 */
void *diffusion(void *ptr);

/**
 * \brief demarre la capture vidéo, et demande à l'utilisateur quel type de capture utiliser.
 * 
 */
int demarrerVideo();

/**
 * \brief arrête la capture vidéo
 * 
 */
int arreterVideo();

/**
 * \brief       Affiche une diffusion d'un serveur
 * \param ptr   Adresse de la socket de dialogue avec le serveur.
 * \note        A utiliser en tant que thread.
 * \warning     La socket doit être bien initialisée
 */
void *regarderDiffusion(void *ptr);

void* afficherDiffusion(void *p);

int recvfrom_bigbuffer(int sock, void *buffer, size_t buflen, int flags,
                       struct sockaddr *src_addr, socklen_t *addrlen);

int sendto_bigbuffer(int sock, const void *buffer, const size_t buflen, int flags,
                     const struct sockaddr *dest_addr, socklen_t addrlen);

