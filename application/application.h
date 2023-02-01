#include "../session/session.h"
#include "../data/data.h"
#include "../protocole/protocole.h"
#include <pthread.h>
#include <ncurses.h>
#include <semaphore.h>

#define NBMAX_THREADS 10
#define BLOC_MEM_LISTE 5 //on realloc la taille de la liste de diffusion toutes les 5 cases
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
void* diffusion();
void viderBuffer();
void dialogueAvecServeur(int sd);

//dans serveurPrincipal.c
void creerProcessusServeur(int se, int sd, struct sockaddr_in *clt);
void * dialogueAvecClient(infoConnexion_t * pInfoConnexion);


/**
 * \brief Trouve les informations d'une diffusion dans la liste de diffusions
 * 
 * \param pListe Liste de diffusion
 * \param id     id de la diffusion à trouver 
 * \return infosDiffusion_t* adresse d'une diffusion
 * \warning on doit avoir la pris la mutex pour éviter les erreurs, de telle façon que l'adresse 
 *          retournée ne soit pas libérée (free) en cas d'arrêt de la diffusion.
 */
infosDiffusion_t * trouverDiffusion(listeDiffusions_t * pListe, long id);

/**
 * \brief Supprime une diffusion
 * 
 * \param pListe Liste de diffusion
 * \param id     id de la diffusion à supprimer
 * \return int   1 si la diffusion était présente, 0 sinon
 */
int supprimerDiffusion(listeDiffusions_t * pListe, long id);

void initListeDiffusions(listeDiffusions_t * pListe);
void insererListeDiffusions(listeDiffusions_t * pListe, demandeAjouterListe_t * pDemandeDiffusion, char addrIP[MAX_DESC]);