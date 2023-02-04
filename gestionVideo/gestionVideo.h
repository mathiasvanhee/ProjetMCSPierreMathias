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

/**
 * \brief informations de connexions utile pour la transmission vidéo
 * 
 */
typedef struct infosConnexion
{
    int sock; /**<socket de connexion */
    struct sockaddr_in * addrSrc;   /**<adresse source*/
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

/**
 * \brief Affiche la diffusion prise par le client lui même. (Retour vidéo du diffuseur)
 */
void* afficherDiffusion(void *p);

/**
 * \brief Permet de recevoir un grand buffer (ici une image) bloc par bloc
 * 
 * \param sock      socket
 * \param buffer    buffer (image)
 * \param buflen    longueur de la chaine
 * \param flags     flags pour recvfrom
 * \param src_addr  Pointeur sur l'adresse distante qui sera remplie si un message est reçu sur la socket <tt>sock</tt>.
 * \param addrlen   Pointeur sur la longueur de l'adresse qui sera remplie si un message est reçu sur la socket <tt>sock</tt>.
 * \return int      -1 si erreur, le nb d'octets lus sinon.
 */
int recvfrom_bigbuffer(int sock, void *buffer, size_t buflen, int flags,
                       struct sockaddr *src_addr, socklen_t *addrlen);

/**
 * \brief Permet d'envoyer un grand buffer (ici une image) bloc par bloc
 * 
 * \param sock      socket
 * \param buffer    buffer (image)
 * \param buflen    longueur de la chaine
 * \param flags     flags pour recvfrom
 * \param dest_addr Pointeur sur l'adresse distante à qui on envoie le buffer.
 * \param addrlen   Longueur de l'adresse distante.
 * \return int      -1 si erreur, le nb d'octets enovoyés sinon.
 */
int sendto_bigbuffer(int sock, const void *buffer, const size_t buflen, int flags,
                     const struct sockaddr *dest_addr, socklen_t addrlen);

