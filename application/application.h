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

/**
 * \brief Affichage et gestion du menu proposé au client.
 * 
 * \return int      0 si l'utilisateur souhaite quitter l'application, 1 sinon. 
 */
int Menu();

/**
 * \brief Affiche la liste des diffusions actives possibles de regarder.
 * \note  Communique avec le serveur principal pour récuperer cette liste,
 *  et lance la fonction permettant de regarder la diffusion sélectionné lorsque
 * l'utilisateur appuie sur la touche [ENTRÉE].
 * 
 */
void afficherListeDiffusion();

/**
 * \brief gère la création d'une diffusion à partir des entrées de l'utilisateur.
 *
 */
void serveurClient();


/**
 * \brief Thread d'écoute pour la diffusion. Attend la connexion des clients
 * distants voulant regarder la diffusion, et lance la fonction de transmission vidéo
 * pour ceux-ci.
 *
 * \param se    adresse de la socket d'écoute.
 */
void* threadEcoute(int* se);

/**
 * \brief fonction qui gère les connexions des clients pour regarder le stream.
 * 
 */
void *connectThread();

void viderBuffer();

/**
 * \brief Permet au client de se connecter à la diffusion d'un autre client, afin de
 * recevoir la diffusion vidéo.
 * 
 * \param idDiff    id de la diffusion à laquelle le client veut se connecter.
 */
void connectDiffusion(long idDiff);

/**
 * \brief Affiche le message permettant à l'utilisateur de savoir
 * quelle touche appuyer pour quitter l'affichage de la liste des diffusions.
 * 
 */
void messageQuitter();

/**
 * \brief Fonction de dialogue du client avec le serveur central.
 * 
 * \param sd socket de dialogue.
 */
void dialogueAvecServeur(int sd);

//dans serveurPrincipal.c


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


/**
 * \brief Initialise la liste des diffusions enregistrées côté serveur.
 * 
 * \param pListe Adresse de la liste de diffusion.
 */
void initListeDiffusions(listeDiffusions_t * pListe);


/**
 * \brief Insère une nouvelle diffusion dans la liste des diffusions enregistrées du serveur principal.
 * 
 * \param pListe            Adresse de la liste de diffusion.
 * \param pDemandeDiffusion Adresse de la requête d'ajout de diffusion dans la liste.
 * \param addrIP            Adresse IP du diffuseur.
 * \return long             <tt>id</tt> de la diffusion insérée.
 */
long insererListeDiffusions(listeDiffusions_t * pListe, demandeAjouterListe_t * pDemandeDiffusion, char addrIP[MAX_DESC]);

/**
 * \brief Remplit une listeInfos_t avec une listeDiffusions_t 
 * 
 * \param pListeSrc  adresse de la liste de diffusion source
 * \param pListeDest adresse de la liste d'informations de diffusion de destinations (contenant seulement l'id et la description de chaque diffusion)
 * \return int       1 si la fonction a réussie, 0 sinon.   
 */
int listeDiffusions_to_listeInfos(listeDiffusions_t * pListeSrc, listeInfos_t * pListeDest);
