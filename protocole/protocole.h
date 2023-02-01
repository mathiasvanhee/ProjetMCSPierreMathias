#include "../data/data.h"

#define MAX_DESC 60

typedef struct infosDiffusion{
    long id;
    char addrIP[16];
    int port;
    char description[MAX_DESC];
} infosDiffusion_t;

typedef struct infoListe{
    long id;
    char description[MAX_DESC];
} infoListe_t;

typedef struct listInfos{
    int taille;
    infoListe_t * tabInfos;
}listeInfos_t;

typedef struct demandeListe{
}demandeListe_t;

typedef enum demandeRetirerListe{
    REASON_DEFAULT,
    REASON_ERROR
} demandeRetirerListe_t;

//pour la demande de nouvelle diffusion, on a juste besoin du port et de la description,
// l'adresse du diffuseur est determinée par le serveur central, et son id est aussi choisie par le serveur central.
typedef struct demandeAjouterListe{
    int port;
    char description[MAX_DESC];
} demandeAjouterListe_t;


typedef enum idReq{
    UNDEFINED,
    LISTE_INFOS,            //Réponse du serveur principal à la demande de la liste des diffusions ouvertes       
    INFOS_DIFFUSION,        //Réponse du serveur principal à la demande des informations de connexion à une diffusion
    DEMANDE_RETIRER_LISTE,  //Demande d'arrêt d'une diffusion d'un client
    DEMANDE_LISTE,          //Demande de la liste des diffusions ouvertes
    DEMANDE_AJOUTER_LISTE,  //Demande de nouvelle diffusion
    SOCKET_CLOSED,          //La socket a été closed
    BAD_REQUEST,            //Réponse du serveur lorsqu'un client a envoyé une requête erronée au serveur (données reçues incohérentes)
    SUCCESS,                //Réponse du serveur signifiant que la requête a été traitée avec succès. 
    ERROR                   //Réponse du serveur signifiant qu'il y a eu une erreur lors du traitement de la requête.
} idReq_t;

typedef struct req{
    idReq_t idReq;
    union{
        listeInfos_t reqListeInfos; //idReq = 1; à malloc
        infosDiffusion_t reqInfosDiffusion;//idReq = 2
        demandeRetirerListe_t reqRetirerListe;//idReq = 3
        demandeListe_t  reqDemandeListe;//idReq = 4
        demandeAjouterListe_t  reqAjouterListe;//idReq = 5
    } r;
} req_t;

void str_to_rep(char *, req_t *);
void req_to_str(req_t *, char *);
void initReqAjouterListe(req_t * req, int port, char * description);