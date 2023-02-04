#include "../session/session.h"
#include "../data/data.h"
#include "../protocole/protocole.h"
#include "../gestionVideo/gestionVideo.h"
#include <pthread.h>
#include <ncurses.h>
#include <curses.h>
#include <semaphore.h>

#define NBMAX_THREADS 10
#define BLOC_MEM_LISTE 5 //on realloc la taille de la liste de diffusion toutes les 5 cases
#define DEBUT_PORT_DGRAM 6000

typedef void * (*pf_t)(void *);

typedef struct infoConnexion{
    int sd;                         //socket de dialogue
    struct sockaddr_in addrClt;     //adresse Client
} infoConnexion_t;

typedef struct listeDiffusions{
    int taille;
    int alloc_len;
    infosDiffusion_t ** tabPInfos; //tableau de pointeurs de diffusions, à malloc à l'insertion
}listeDiffusions_t;

//dans client.c
int Menu();
void afficherListeDiffusion();
void serveurClient();
void* threadEcoute(int* se);
void *connectThread();
void viderBuffer();
void dialogueAvecServeur(int sd);

//dans serveurPrincipal.c
void creerProcessusServeur(int se, int sd, struct sockaddr_in *clt);
void * dialogueAvecClient(infoConnexion_t * pInfoConnexion);

/**
 * \brief Trouve les informations d'une diffusion dans la liste de diffusions
 * 
 * \param pListe Adresse de la liste de diffusion
 * \param pInfos Adresse des informations de diffusion à remplir
 * \param id     id de la diffusion à trouver 
 * \return int   1 si la diffusion a été trouvée, 0 sinon.
 */
int getDiffusion(listeDiffusions_t * pListe, infosDiffusion_t * pInfos,  long id);

/**
 * \brief Supprime une diffusion
 * 
 * \param pListe Liste de diffusion
 * \param id     id de la diffusion à supprimer
 * \return int   1 si la diffusion était présente, 0 sinon
 */
int supprimerDiffusion(listeDiffusions_t * pListe, long id);

void initListeDiffusions(listeDiffusions_t * pListe);
long insererListeDiffusions(listeDiffusions_t * pListe, demandeAjouterListe_t * pDemandeDiffusion, char addrIP[MAX_DESC]);

/**
 * \brief Remplit une listeInfos_t avec une listeDiffusions_t 
 * 
 * \param pListeSrc  adresse de la liste de diffusion source
 * \param pListeDest adresse de la liste d'informations de diffusion de destinations (contenant seulement l'id et la description de chaque diffusion)
 * \return int 
 */
int listeDiffusions_to_listeInfos(listeDiffusions_t * pListeSrc, listeInfos_t * pListeDest);


void connectDiffusion(long idDiff);

void messageQuitter();