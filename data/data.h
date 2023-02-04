#ifndef DATA_H
#define DATA_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>

#ifndef CHECK
#define CHECK(sts, msg) \
    if ((sts) == -1)    \
    {                   \
        perror(msg);    \
        exit(-1);       \
    }
#endif

#ifndef PAUSE
#define PAUSE(msg) printf("%s [touche]", msg); getchar();
#endif

#define MAX_BUFF 1024

typedef void (fct_Serial)(void*,void *);

/**
 * \brief Handler de signaux, si SIGCHLD reçu, acquitte le processus fils.
 * 
 * \param sig signal reçu.
 */
void traiterSIGCHLD(int sig);

/**
 * \brief Lis un datagramme UDP.
 * 
 * \param sock      socket
 * \param src       Pointeur sur l'adresse source d'envoi du datagramme,
 * est modifiée par la fonction afin qu'elle contienne la bonne adresse source
 * \param buffer    Buffer rempli par la fonction, qui reçoit la chaine de caractères envoyée.
 * \return int      Le nombre d'octets reçus, ou -1 s'il y a une erreur.
 */
int lireDgram(int sock, struct sockaddr_in * src, char * buffer);

/**
 * \brief Envoie un datagramme UDP.
 * 
 * \param sock      socket
 * \param dest      Pointeur sur l'adresse de destination d'envoi du datagramme,
 * \param buffer    Chaine de caractères à envoyer.
 * \return int      Le nombre d'octets envoyés, ou -1 s'il y a une erreur.
 */
int ecrireDgram(int sock, struct sockaddr_in * dest, char * buffer);

/**
 * \brief Lis un segment STREAM (tcp).
 * 
 * \param sock      socket
 * \param buffer    Buffer rempli par la fonction, qui reçoit la chaine de caractères envoyée.
 * \return int      Le nombre d'octets reçus, ou -1 s'il y a une erreur.
 */
int lireStream(int sock, char * buffer);

/**
 * \brief Envoie un segment STREAM (tcp).
 * 
 * \param sock      socket
 * \param buffer    Chaine de caractères à envoyer.
 * \return int      Le nombre d'octets envoyés, ou -1 s'il y a une erreur.
 */
int ecrireStream(int sock, char * buffer);

/**
 * \brief Envoie une requête en stream.
 * 
 * \param sock          Socket de dialogue.
 * \param req           Pointeur sur la requête à envoyer.
 * \param reqToSerial   Pointeur sur une fonction qui permet de serialiser la requête.
 */
void envoyerReqStream(int sock, void *req, fct_Serial * reqToSerial);

/**
 * \brief Envoie une requête en Datagram.
 * 
 * \param sock          Socket
 * \param req           Pointeur sur la requête à envoyer.
 * \param reqToSerial   Pointeur sur une fonction qui permet de serialiser la requête.
 * \param addr           Pointeur sur l'adresse de destination d'envoi du datagramme,
 */
void envoyerReqDgram(int sock, void *req, fct_Serial * reqToSerial, struct sockaddr_in *addr);

/**
 * \brief Reçoie une requête en Stream.
 * 
 * \param sock          Socket de dialogue.
 * \param req           Pointeur sur la requête remplie à la reception.
 * \param serialToRep   Pointeur sur une fonction qui permet de déserialiser la chaine de caractère en requête.
 */
void lireRepStream(int sock, void *req, fct_Serial * serialToRep);

/**
 * \brief Reçoie une requête en datagram.
 * 
 * \param sock          Socket.
 * \param req           Pointeur sur la requête remplie à la reception.
 * \param serialToRep   Pointeur sur une fonction qui permet de déserialiser la chaine de caractère en requête.
 * \param src           Pointeur sur l'adresse source d'envoi du datagramme,
 * est modifiée par la fonction afin qu'elle contienne la bonne adresse source
 */
void lireRepDgram(int sock, void *req, fct_Serial * serialToRep, struct sockaddr_in *src);

#endif