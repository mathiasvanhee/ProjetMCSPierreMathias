#include "../session/session.h"
#include "../data/data.h"
#include "../protocole/protocole.h"
#include <pthread.h>
#include <ncurses.h>

#define NBMAX_THREADS 10
typedef void * (*pf_t)(void *);

//dans client.c
void dialogueAvecServeur(int sd);
int Menu();
void afficherListeDiffusion();
void serveurClient();
void* threadEcoute(int* se);
void *connectThread();
void* diffusion();
void viderBuffer();

//dans serveurPrincipal.c
void creerProcessusServeur(int se, int sd, struct sockaddr_in *clt);
void dialogueAvecClient(int sd, struct sockaddr_in *clt);