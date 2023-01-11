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

void traiterSIGCHLD(int sig);
void creerProcessusServeur(int se, int sd, struct sockaddr_in * clt);
int lireDgram(int sock, struct sockaddr_in * src, char * buffer);
int ecrireDgram(int sock, struct sockaddr_in * dest, char * buffer);
int lireStream(int sock, char * buffer);
int ecrireStream(int sock, char * buffer);
void dialogueAvecClient(int sd, struct sockaddr_in * clt);
void dialogueAvecServeur(int sd);