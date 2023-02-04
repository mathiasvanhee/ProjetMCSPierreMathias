
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

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


int creerSocket(int type);

/**
 * \brief Remplit l'adresse locale de la socket
 * 
 * \param sock 
 * \param IPaddr 
 * \param port 
 * \return int 
 */
int adresserSocket(int sock, char * IPaddr, short port);

/**
 * \brief Creer la socket et remplit l'adresse locale de celle-ci
 * 
 * \param type 
 * \param IPaddr 
 * \param port 
 * \return int 
 */
int creerSocketAddr(int type, char * IPaddr, short port);
int creerSockAddrEcoute(char * IPaddr, short port, int maxfile);
int attenteAppel(int sockEcoute, struct sockaddr_in * pClt);
void connectSrv(int sock, char * IPaddr, short port);
void initAddr(struct sockaddr_in * pAddr, char * IPaddr, short port);

